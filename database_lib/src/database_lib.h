#pragma once

#include <iostream>
#include <string>
#include <memory>
#include "sql_builder.h"

struct ConnectParameters
{
	std::string host;
	std::string port;
	std::string instance;
	std::string userName;
	std::string password;
};

void testDB(const ConnectParameters & cp);

class DBExecutor
{
public:
	DBExecutor(const ConnectParameters & cp);
	~DBExecutor();	
	template <typename... T>
	int execute_procedure(const std::string & procedure, const T &... p)
	{
		SqlBuilder sqb(procedure);
		build_sql(sqb, p...);		
		return execute_procedure(sqb);
	}
private:
	struct DBExecutorImpl;
	std::unique_ptr<DBExecutorImpl> impl;
	int execute_procedure(SqlBuilder & sqb);
	SqlBuilder & build_sql(SqlBuilder & sqb)
	{
		return sqb;
	}
	template<typename T, typename... Args>
	SqlBuilder & build_sql(SqlBuilder & sqb, const T & p, const Args &... rest)
	{
		sqb << p;
		return build_sql(sqb, rest...);
	}
};

