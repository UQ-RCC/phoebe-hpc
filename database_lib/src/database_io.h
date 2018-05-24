#pragma once

#include <libpq-fe.h>
#include "oid.h"
#include <deque>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <nlohmann/json.hpp>
#include <fmt/printf.h>
#include "database_lib.h"

#define DATA(x)   extern int no_such_variable
#define DESCR(x)  extern int no_such_variable
#define SHDESCR(x) extern int no_such_variable

DATA(insert OID = 16 (bool	   PGNSP PGUID	1 t b B t t \054 0	 0 1000 boolin boolout boolrecv boolsend - --c p f 0 - 1 0 0 _null_ _null_ _null_));
DESCR("boolean, 'true'/'false'");
#define BOOLOID			16



class MeshMakerDB
{
private:
	MeshMakerDB();
	bool checkStmt(PGresult * result, PGconn * conn);
	PGconn * meshConn;
	PGconn * frameConn;	
	boost::mutex meshConnMtx;
	boost::mutex frameConnMtx;
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
public:
	MeshMakerDB(const ConnectParameters & params);
	~MeshMakerDB();
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

};