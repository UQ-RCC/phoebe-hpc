#pragma once
#include <iostream>

#ifdef _WIN32
	#define EXIT(C) {std::cout << "enter to continue." << std::endl; std::cin.get(); exit(C);}
#else
	#define EXIT(C) exit(C)
#endif