#include "database_io.h"

phoebe_database::phoebe_database(const connect_parameters & params)
{
	
	meshConn = PQsetdbLogin(
		params.host.c_str(),
		params.port.c_str(), 
		NULL, 
		NULL,
		params.instance.c_str(),
		params.userName.c_str(),
		params.password.c_str());

	ConnStatusType status = PQstatus(meshConn);
	if (status != CONNECTION_OK)
	{
		throw std::runtime_error("no database connection");
	}

	frameConn = PQsetdbLogin(
		params.host.c_str(),
		params.port.c_str(),
		NULL,
		NULL,
		params.instance.c_str(),
		params.userName.c_str(),
		params.password.c_str());
}

phoebe_database::~phoebe_database()
{
	PQfinish(meshConn);
	PQfinish(frameConn);
}

void phoebe_database::test_connection()
{
	boost::lock_guard<boost::mutex> lock(meshConnMtx);
	fmt::print("testing");
	PGresult * testRS = PQexec(meshConn, "select version()");
	if (checkStmt(testRS, meshConn))
	{
		if (!PQgetisnull(testRS, 0, 0))
		{
			fmt::print("{}\n", PQgetvalue(testRS, 0, 0));
		}
		else
		{
			fmt::print("no results\n");
		}
	}
	PQclear(testRS);
}

int phoebe_database::execute_sqb(SqlBuilder & sqb)
{	
	int returnValue = -1;
	PGresult * testRS = PQexecParams(meshConn, sqb.get_sql().c_str(), sqb.NumParameters(), NULL, sqb.GetParameter(), (int *) sqb.GetParamLength(), sqb.GetParamFormat(), 0);
	if (checkStmt(testRS, meshConn))
	{
		if (!PQgetisnull(testRS, 0, 0))
		{
			returnValue = atoi(PQgetvalue(testRS, 0, 0));
		}		
	}
	PQclear(testRS);
	return returnValue;
}

// TODO evaluate and decide what to do with json output...
nlohmann::json phoebe_database::execute_sqb_json(SqlBuilder & sqb)
{
	nlohmann::json json;
	PGresult * testRS = PQexecParams(meshConn, sqb.get_sql().c_str(), sqb.NumParameters(), NULL, sqb.GetParameter(), (int *)sqb.GetParamLength(), sqb.GetParamFormat(), 0);
	if (checkStmt(testRS, meshConn))
	{
		int num_rows = PQntuples(testRS);
		if (num_rows > 0)
		{
			if (!PQgetisnull(testRS, 0, 0))
			{
				int num_fields = PQnfields(testRS);
				for (int i = 0; i < num_fields; i++)
				{
					json[PQfname(testRS, i)] = PQgetvalue(testRS, 0, i);
					json["type"] = PQftype(testRS, i);
				}				
			}
		}
	}
	PQclear(testRS);
	return json;
}

bool phoebe_database::checkStmt(PGresult * result, PGconn * conn)
{
	if (PQresultStatus(result) != PGRES_TUPLES_OK)
	{
		fprintf(stderr, "SLQ error: %s\n", PQerrorMessage(conn));		
		fprintf(stderr, "State: %s\n", PQresultErrorField(result, PG_DIAG_SQLSTATE));
		fprintf(stderr, "Primary: %s\n", PQresultErrorField(result, PG_DIAG_MESSAGE_PRIMARY));
		fprintf(stderr, "Detail: %s\n", PQresultErrorField(result, PG_DIAG_MESSAGE_DETAIL));
		fprintf(stderr, "Pos: %s\n", PQresultErrorField(result, PG_DIAG_STATEMENT_POSITION));
		fprintf(stderr, "Context: %s\n", PQresultErrorField(result, PG_DIAG_CONTEXT));		
		return false;
	}
	return true;
}
