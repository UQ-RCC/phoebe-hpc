#include "sql_builder.h"

SqlBuilder::SqlBuilder(const std::string & function) : function(function) {}

SqlBuilder& SqlBuilder::operator<<(const std::string & param)
{
	length.push_back(param.length());
	format.push_back(0);
	stringKeeper.push_back(param);
	parameter.push_back(stringKeeper.back().c_str());
	parameterDef.push_back(std::string().append("$").append(std::to_string(parameter.size())));
	return *this;
}

SqlBuilder& SqlBuilder::operator<<(const int param)
{
	length.push_back(sizeof(int));
	format.push_back(1);
	intKeeper.push_back(mhtonl(param));
	parameter.push_back((char *)&intKeeper.back());
	parameterDef.push_back(std::string().append("$").append(std::to_string(parameter.size())).append("::integer"));
	return *this;
}

SqlBuilder& SqlBuilder::operator<<(const double param)
{
	length.push_back(sizeof(double));
	format.push_back(1);
	doubleKeeper.push_back(0.0);
	to_nbo(param, &doubleKeeper.back());
	parameter.push_back((char *)&doubleKeeper.back());
	parameterDef.push_back(std::string().append("$").append(std::to_string(parameter.size())).append("::double precision"));
	return *this;
}

std::string SqlBuilder::getSQL()
{
	std::string sql = "select * from ";
	sql.append(function).append("(");
	for (int i = 0; i < (NumParameters() - 1); i++)
	{	
		std::cin.get();
		sql.append(parameterDef[i]);
		sql.append(", ");
	}
	if (NumParameters() > 0)
	{
		sql.append(parameterDef[NumParameters() - 1]);
	}
	sql.append(")");	
	return sql;
}

size_t * SqlBuilder::GetParamLength()
{
	return NumParameters() ? length.data() : NULL;
}

int * SqlBuilder::GetParamFormat()
{
	return NumParameters() ? format.data() : NULL;
}

const char ** SqlBuilder::GetParameter()
{
	return NumParameters() ? parameter.data() : NULL;
}

int SqlBuilder::NumParameters()
{
	return (int) parameter.size();
}

void SqlBuilder::to_nbo(const double in, double * out)
{
	uint64_t * i = (uint64_t *)&in;
	uint32_t * r = (uint32_t *)out;

	/* convert input to network byte order */
	//r[0] = mhtonl((uint32_t)((*i) >> 32));
	//r[1] = mhtonl((uint32_t)*i);
}

int SqlBuilder::mhtonl(int p)
{
	return p;
}

int htonl(const uint32_t net)
{
	uint8_t data[4] = {};
	memcpy(&data, &net, sizeof(data));
	return 0;
}