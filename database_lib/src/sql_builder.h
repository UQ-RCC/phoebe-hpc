#pragma once

#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <stdint.h>
#include <cstring>
#include "endian_util.h"
#include "fmt/printf.h"

class SqlBuilder
{
public:
	SqlBuilder(const std::string & function);
	SqlBuilder& operator<<(const std::string & param);
	SqlBuilder& operator<<(const int param);
	SqlBuilder& operator<<(const double param);
	SqlBuilder& operator<<(const float param);
	std::string get_sql();
	int * GetParamLength();
	int * GetParamFormat();
	const char ** GetParameter();
	int NumParameters();
private:	
	std::string function;
	std::vector<int> length;
	std::vector<int> format;
	std::vector<const char *> parameter;
	std::vector<std::string> parameterDef;
	// store a copies of data so that we can push pointers.
	std::deque<int> intKeeper;
	std::deque<float> floatKeeper;
	std::deque<double> doubleKeeper;
	std::deque<std::string> stringKeeper;
};