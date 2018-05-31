#include "options.h"
#include "sb_io.h"
#include "itkVersion.h"
#include "itkImportImageFilter.h"

void ConvertSBImages(const Options & options);

int main(int argc, char ** argv)
{
	itk::Version * ver = itk::Version::New();
	auto v = ver->GetITKVersion();
	fmt::print("Slidebook converter v0.1 (ITK version: {})\n", v);
	Options options(argc, argv);
	options.Print();
	ConvertSBImages(options);
	fmt::print("done\n");
	EXIT(0);
}

void ConvertSBImages(const Options & options) try
{
	
	boost::filesystem::path input_path(options.data_source);

	if (!boost::filesystem::exists(input_path))
	{
		fmt::print("{} not found\n", input_path);
		EXIT(1);
	}	
	
	auto sb_read_file = III_NewSBReadFile(input_path.make_preferred().string().c_str(), III::kNoExceptionsMasked);	
	if (options.verbose)
	{
		fmt::print("sb file loaded\n");
	}
	
	auto captures = sb_read_file->GetNumCaptures();
	if (options.verbose)
	{
		fmt::print("captures: {}\n", captures);
	}

	using PixelType = UInt16;
	const int Dimension = 3;
	using ImageType = itk::Image<PixelType, Dimension>;

	SB_IO<ImageType> sb_io(options);
	CaptureIndex number_captures = sb_read_file->GetNumCaptures();

	phoebe_database * db;

	if (options.destination_protocol == Protocol::OBJECT_STORE)
	{
		db = new phoebe_database(options.db_parameters);
	}
	
	for (int capture_index = 0; capture_index < number_captures; capture_index++)
	{

		CaptureDataFrame cp(sb_read_file, capture_index, 0);
		if (options.verbose)
		{
			fmt::print("{}\n", cp.GetHeader(capture_index, cp.position_index));
		}

		using ImageFilterType = itk::ImportImageFilter<PixelType, Dimension>;
		using WriterType = itk::ImageFileWriter<ImageType>;
		using TIFFIOType = itk::TIFFImageIO;

		std::size_t planeSize = cp.xDim * cp.yDim;
		int bufferSize = cp.xDim * cp.yDim * cp.zDim;
		PixelType * buffer = new UInt16[bufferSize];

		ImageType * image = ImageType::New();
		ImageType::IndexType corner = { { 0, 0, 0 } };

		auto imageImport = ImageFilterType::New();

		ImageFilterType::SizeType image_size;
		image_size[0] = cp.xDim;
		image_size[1] = cp.yDim;
		image_size[2] = cp.zDim;
		//ImageFilterType::SizeType image_size_1{ cp.xDim , cp.yDim , cp.zDim };

		ImageFilterType::IndexType start;
		start.Fill(0);
		ImageFilterType::RegionType region;
		region.SetIndex(start);
		region.SetSize(image_size);
		imageImport->SetRegion(region);
		const itk::SpacePrecisionType origin[3] = { 0.0, 0.0, 0.0 };
		imageImport->SetOrigin(origin);
		//const itk::SpacePrecisionType  spacing[3] = { cp.voxel_size[0], cp.voxel_size[1], cp.voxel_size[2] };
		const itk::SpacePrecisionType  spacing[3] = { 1.0, 1.0, 1.0 };
		imageImport->SetSpacing(spacing);
		imageImport->SetImportPointer(buffer, bufferSize, false);

		std::string experiment_name;
		std::string folder_name;

		if (options.destination_protocol == Protocol::OBJECT_STORE)
		{
			if (cp.number_captures == 1)
			{
				folder_name.clear();
				folder_name.assign("folder");
				experiment_name = input_path.stem().string();			
			}
			else
			{
				folder_name = input_path.stem().string();
				experiment_name = cp.image_name;			
			}

			db->execute_proc("q_log", "test_client", "{}");
						
			auto [experiment_id] = db->execute_proc<int>("insert_experiment",
				experiment_name,
				folder_name,
				cp.xDim,
				cp.yDim,
				cp.zDim,
				cp.number_channels,
				cp.number_timepoints,
				cp.voxel_size[0],
				cp.voxel_size[1],
				cp.voxel_size[2])[0];

			fmt::print("exp: '{}', folder: '{}', id: {} [{} {} {}]\n", 
				experiment_name, folder_name, experiment_id,
				cp.voxel_size[0], cp.voxel_size[1], cp.voxel_size[2]);
			
		}

		int cappedTime = cp.number_timepoints;
		if (options.max_time > -1)
		{
			cappedTime = std::min(cappedTime, options.max_time);
		}

		for (int timepoint_index = 0; timepoint_index < cappedTime; timepoint_index++)
		{
			cp.timepoint_index = timepoint_index;
			for (int c = 0; c < cp.number_channels; c++)
			{
				cp.channels_index = c;
				for (int z = 0; z < cp.zDim; z++)
				{
					sb_read_file->ReadImagePlaneBuf(buffer + (z * planeSize), capture_index, 0, timepoint_index, z, c);
				}
				if (!options.debug)
				{
					sb_io.writeFile(cp, "", "tiff", false, imageImport->GetOutput());
				}				
			}
			if (options.verbose)
			{
				fmt::print("processed timepoint {}\n", cp.GetTimepointIndexString());
				if (options.destination_protocol == Protocol::OBJECT_STORE)
				{
					fmt::print("send to database\n");
				}
			}
		}
		delete[] buffer;
	
	}

	delete db;

}
catch (const III::Exception * e)
{
	std::cout << "Failed with exception: " << e->GetDescription() << std::endl;
	std::string inString;
	std::cout << "Press Enter." << std::endl;
	std::getline(std::cin, inString);
	delete e;
}