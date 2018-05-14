// phoebe-hpc.cpp : Defines the entry point for the application.
//

#include "test.h"
#include <boost/version.hpp>

int main()
{
	std::cout << "Testing Phoebe components...\n\n";
	std::cout << "boost lib version: " << BOOST_LIB_VERSION << std::endl;
#ifdef _WIN32
	std::cout << "\nEnter to exit" << std::endl;
	std::cin.get();
#endif // _WIN32
	return 0;
}
