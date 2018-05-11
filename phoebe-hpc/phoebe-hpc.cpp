// phoebe-hpc.cpp : Defines the entry point for the application.
//

#include "phoebe-hpc.h"
#include <boost/version.hpp>

int main()
{
	std::cout << "boost lib version: " << BOOST_LIB_VERSION << std::endl;
	std::cin.get();
	return 0;
}
