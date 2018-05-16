#include "test.h"

int main()
{
	std::cout << "Testing Phoebe components.\n\n";
	std::cout << "Boost lib version: " << BOOST_LIB_VERSION << std::endl;
	itk::Version * ver = itk::Version::New();
	auto v = ver->GetITKVersion();	
	std::cout << "ITK lib version: " << v << std::endl;
	fmt::print("Format lib version {}\n", FMT_VERSION);
	std::cout << std::endl;
	EXIT(0);
}