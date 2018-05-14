#include "options.h"

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