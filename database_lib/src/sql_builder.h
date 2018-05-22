#pragma once

#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <stdint.h>

class SqlBuilder
{
public:
	SqlBuilder(const std::string & function);
	SqlBuilder& operator<<(const std::string & param);
	SqlBuilder& operator<<(const int param);
	SqlBuilder& operator<<(const double param);
	std::string getSQL();
	size_t * GetParamLength();
	int * GetParamFormat();
	const char ** GetParameter();
	int NumParameters();
private:
	void to_nbo(const double in, double * out);
	std::string function;
	std::vector<size_t> length;
	std::vector<int> format;
	std::vector<const char *> parameter;
	std::vector<std::string> parameterDef;
	// store a copies of data so that we can push pointers.
	std::deque<int> intKeeper;
	std::deque<double> doubleKeeper;
	std::deque<std::string> stringKeeper;
	int mhtonl(int p);
};