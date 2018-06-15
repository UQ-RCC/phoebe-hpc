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
	int tmp_i;
	phoebe::htoni(&param, &tmp_i);
	intKeeper.push_back(tmp_i);	
	parameter.push_back((char *)&intKeeper.back());
	int tmp = *(&intKeeper.back());
	fmt::print("<< int : {} {} {}\n", param, intKeeper.back(), phoebe::ntohl(intKeeper.back()));
	parameterDef.push_back(std::string().append("$").append(std::to_string(parameter.size())).append("::integer"));
	return *this;
}

SqlBuilder& SqlBuilder::operator<<(const double param)
{
	length.push_back(sizeof(double));
	format.push_back(1);
	double tmp_d;
	phoebe::htonll(&param, &tmp_d);	
	doubleKeeper.push_back(tmp_d);
	parameter.push_back((char *)&doubleKeeper.back());
	fmt::print("<< double : {} {} {}\n", param, doubleKeeper.back(), phoebe::ntohll(doubleKeeper.back()));
	parameterDef.push_back(std::string().append("$").append(std::to_string(parameter.size())).append("::double precision"));
	return *this;
}

SqlBuilder& SqlBuilder::operator<<(const float param)
{
	length.push_back(sizeof(float));
	format.push_back(1);	
	float tmp_d;
	phoebe::htonl(&param, &tmp_d);
	floatKeeper.push_back(tmp_d);
	parameter.push_back((char *)&floatKeeper.back());
	fmt::print("<< float : {} {} {}\n", param, floatKeeper.back(), phoebe::ntohl(floatKeeper.back()));
	parameterDef.push_back(std::string().append("$").append(std::to_string(parameter.size())).append("::real"));
	return *this;
}

std::string SqlBuilder::get_sql()
{
	std::string sql = "select * from ";
	sql.append(function).append("(");
	for (int i = 0; i < (NumParameters() - 1); i++)
	{			
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

int * SqlBuilder::GetParamLength()
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
