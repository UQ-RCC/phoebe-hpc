#pragma once

#include <libpq-fe.h>
#include <deque>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <nlohmann/json.hpp>
#include <fmt/printf.h>
#include "database_lib.h"

class MeshMakerDB
{
private:
	MeshMakerDB();
	bool checkStmt(PGresult * result, PGconn * conn);
	PGconn * meshConn;
	PGconn * frameConn;	
	boost::mutex meshConnMtx;
	boost::mutex frameConnMtx;
public:
	MeshMakerDB(const ConnectParameters & params);
	~MeshMakerDB();
	void test_connection();	
	int execute_sqb(SqlBuilder & sqb);
	nlohmann::json execute_sqb_json(SqlBuilder & sqb);
	int execute();	
};