#include "options.h"
#include "SBReadFile.h"

int main(int argc, char ** argv)
{
	//itk::Version * ver = itk::Version::New();
	//auto v = ver->GetITKVersion();
	//fmt::print("Slidebook converter v0.1 (ITK version: {})\n", v);
	Options options(argc, argv);
	options.Print();
	//ConvertSBImages(options);
	//fmt::print("done\n");
	EXIT(0);
}

void ConvertSBImages(const Options & options)
{
	/*
	
	boost::filesystem::path inputPath(options.data_source);

	if (!boost::filesystem::exists(inputPath))
	{
		fmt::print("{} not found\n", inputPath);
		EXIT(1);
	}

	UInt32 inExceptionMask = 0;
	auto sb_read_file = III_NewSBReadFile(inputPath.make_preferred().string().c_str(), inExceptionMask);
	auto captures = sb_read_file->GetNumCaptures();

	using PixelType = UInt16;
	const int Dimension = 3;
	using ImageType = itk::Image<PixelType, Dimension>;

	SB_IO<ImageType> sb_io(options);
	CaptureIndex number_captures = sb_read_file->GetNumCaptures();

	for (int capture_index = 0; capture_index < number_captures; capture_index++)
	{
		CaptureDataFrame cp(sb_read_file, capture_index);
		if (options.verbose)
		{
			fmt::print("{}\n", cp.GetHeader(capture_index, cp.position_index));
		}

		if (options.debug)
		{
			continue;
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
		ImageFilterType::SizeType image_size_1{ cp.xDim , cp.yDim , cp.zDim };

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
				sb_io.writeFile(cp, "", "tiff", false, imageImport->GetOutput());
				//sb_io.writeFile("_deflate", "tiff", true, imageImport->GetOutput());
			}
		}
		delete[] buffer;
	}
	*/
}


