#include "database_lib.h"
#include "database_io.h"

using namespace std;

void testDB(const ConnectParameters & cp)
{	
	//MeshMakerDB db(cp);
	//db.test_connection();
}

struct DBExecutor::DBExecutorImpl
{
public:
	DBExecutorImpl(const ConnectParameters & cp) : db(cp) {};	
	~DBExecutorImpl() {};
	int execute_procedure(SqlBuilder & sqb)
	{
		return db.execute_sqb(sqb);		
	}
	nlohmann::json execute_procedure_json(SqlBuilder & sqb)
	{
		return db.execute_sqb_json(sqb);
	}

private:
	MeshMakerDB db;
};

DBExecutor::DBExecutor(const ConnectParameters & cp) : impl(new DBExecutorImpl(cp)) {}
DBExecutor::~DBExecutor() {}

int DBExecutor::execute_procedure(SqlBuilder & sqb)
{	
	return impl->execute_procedure(sqb);
}

nlohmann::json DBExecutor::execute_procedure_json(SqlBuilder & sqb)
{	
	return impl->execute_procedure_json(sqb);
}
