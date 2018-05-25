#pragma once

#include <libpq-fe.h>
#include "oid.h"
#include <deque>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <nlohmann/json.hpp>
#include <fmt/printf.h>
#include "sql_builder.h"

struct ConnectParameters
{
	std::string host;
	std::string port;
	std::string instance;
	std::string userName;
	std::string password;
};

class PhoebeDatabase
{
private:
	PhoebeDatabase();
	bool checkStmt(PGresult * result, PGconn * conn);
	PGconn * meshConn;
	PGconn * frameConn;	
	boost::mutex meshConnMtx;
	boost::mutex frameConnMtx;

	// Excecute sql procedure
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

	//new stuff
	PGresult * get_result_set(std::string statement);
	

public:
	PhoebeDatabase(const ConnectParameters & params);
	~PhoebeDatabase();
	void test_connection();		
	int execute();

	int execute_sqb(SqlBuilder & sqb);

	template <typename... T>
	int execute_procedure(const std::string & procedure, const T &... p)
	{
		SqlBuilder sqb(procedure);
		build_sql(sqb, p...);
		return execute_sqb(sqb);
	}

	nlohmann::json execute_sqb_json(SqlBuilder & sqb);

	template <typename... T>
	nlohmann::json execute_procedure_json(const std::string & procedure, const T &... p)
	{
		SqlBuilder sqb(procedure);
		build_sql(sqb, p...);
		return execute_sqb_json(sqb);
	}


	// new stuff

	template<typename... R>
	std::vector<std::tuple<R...>> execute_statement(std::string statement)
	{
		std::vector<std::tuple<R...>> result_vector;
		PGresult * result = PQexec(meshConn, statement.c_str());
		int num_rows = PQntuples(result);
		int num_fields = PQnfields(result);
		int num_output_fields = sizeof...(R);
		if (num_output_fields > num_fields)
		{
			throw std::runtime_error(fmt::format("too many output fields {} bound to query with {} return fields", num_output_fields, num_fields));
		}
		fmt::print("got {} rows {} in fields {} out fields\n", num_rows, num_fields, num_output_fields);

		PQclear(result);
		return result_vector;
	}



};