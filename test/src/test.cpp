#include "test.h"

int main()
{	
	fmt::print("Testing Phoebe components.\n\n");
	fmt::print("Boost lib version: {}", BOOST_LIB_VERSION);
	itk::Version * ver = itk::Version::New();
	auto v = ver->GetITKVersion();	
	std::cout << "ITK lib version: " << v << std::endl;
	fmt::print("Format lib version {}\n", FMT_VERSION);
	fmt::print("JSON lib version {}.{}.{}\n", NLOHMANN_JSON_VERSION_MAJOR, NLOHMANN_JSON_VERSION_MINOR, NLOHMANN_JSON_VERSION_PATCH);
	fmt::print("Postgres client version {}\n", PQlibVersion());
	std::cout << std::endl;
	EXIT(0);
}