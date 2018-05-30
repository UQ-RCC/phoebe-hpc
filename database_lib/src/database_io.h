#pragma once

#include <libpq-fe.h>
#include "oid.h"

#include <cstdlib>
#include <deque>
#include <boost/thread.hpp>
#include <nlohmann/json.hpp>
#include <fmt/printf.h>
#include <tuple>
#include "sql_builder.h"

struct connect_parameters
{
	std::string host;
	std::string port;
	std::string instance;
	std::string userName;
	std::string password;
};

inline uint32_t ntohl(uint32_t const net)
{
	uint8_t * p_net = (uint8_t *) &net;
	return ((uint32_t)p_net[0] << 24)
		| ((uint32_t)p_net[1] << 16)
		| ((uint32_t)p_net[2] << 8)
		| ((uint32_t)p_net[3] << 0);
}

inline uint32_t htonl(uint32_t const host)
{
	uint32_t net;
	uint8_t * p_net = (uint8_t *) &net;
	p_net[0] = (uint8_t)(host >> 24);
	p_net[1] = (uint8_t)(host >> 16);
	p_net[2] = (uint8_t)(host >> 8);
	p_net[3] = (uint8_t)(host >> 0);
	return net;
}

inline uint64_t ntohll(uint64_t const net)
{
	uint8_t * p_net = (uint8_t *) &net;
	return ((uint64_t)p_net[0] << 56)
		| ((uint64_t)p_net[1] << 48)
		| ((uint64_t)p_net[2] << 40)
		| ((uint64_t)p_net[3] << 32)
		| ((uint64_t)p_net[4] << 24)
		| ((uint64_t)p_net[5] << 16)
		| ((uint64_t)p_net[6] << 8)
		| ((uint64_t)p_net[7] << 0);	
}

/*
inline uint64_t htonll(uint64_t const host)
{
	uint64_t net;
	uint32_t * p_net = (uint32_t *) &net;
	p_net[0] = (uint32_t)(host >> 32);
	p_net[1] = (uint32_t)(host >> 0);
	return net;
}
*/

inline std::string get_hex(uint64_t i)
{
	return fmt::format("{:02x} {:02x} {:02x} {:02x} {:02x} {:02x} {:02x} {:02x}",
		(uint8_t)(i >> 56),
		(uint8_t)(i >> 48),
		(uint8_t)(i >> 40),
		(uint8_t)(i >> 32),
		(uint8_t)(i >> 24),
		(uint8_t)(i >> 16),
		(uint8_t)(i >> 8),
		(uint8_t)i
	);
}

inline std::string get_hex64(uint8_t * i)
{
	return fmt::format("{:02x} {:02x} {:02x} {:02x} {:02x} {:02x} {:02x} {:02x}",
		i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]);
}

template<typename... R>
class result_set
{
private:
	PGresult * result;

	void set_value(long & v, int row, int column)
	{
		v = atol(PQgetvalue(result, row, column));
	}
	
	void set_value(int & v, int row, int column)
	{
		v = atoi(PQgetvalue(result, row, column));		
	}

	void set_value(double & v, int row, int column)
	{
		v = atof(PQgetvalue(result, row, column));
	}

	void set_value(std::string & v, int row, int column)
	{
		v.assign(PQgetvalue(result, row, column));
	}

	template<typename P>
	std::tuple<P> get_tuple(int row, int num_parameters)
	{
		int remaining_parametes = 1;
		P v;
		set_value(v, row, num_parameters - remaining_parametes);
		return std::make_tuple(v);
	}
	
	template<typename P, typename F, typename ... R>
	std::tuple<P, F, R...> get_tuple(int row, int num_parameters)
	{
		int remaining_parametes = sizeof...(R) + 2;
		P v;
		set_value(v, row, num_parameters - remaining_parametes);
		return std::tuple_cat(std::make_tuple(v), get_tuple<F, R...>(row, num_parameters));
	}

	template<typename P, typename F, typename ... R>
	std::tuple<P, F, R...> get_tuple(int row)
	{
		int num_parameters = sizeof...(R) + 2;
		P v;
		set_value(v, row, 0);
		return std::tuple_cat(std::make_tuple(v), get_tuple<F, R...>(row, num_parameters));
	}

	template<typename P>
	std::tuple<P> get_tuple(int row)
	{
		P v;
		set_value(v, row, 0);
		return std::make_tuple(v);
	}
	
public:
	int size = 0;
	int num_fields = 0;
	bool is_empty = true;

	result_set(PGresult * result)
	{
		this->result = result;
		size = PQntuples(result);
		if (size > 1)
		{
			is_empty = false;
		}
		num_fields = PQnfields(result);
	}

	~result_set()
	{
		fmt::print("result deleted.\n");
		PQclear(result);
	}

	std::tuple<R...> operator[] (int row)
	{
		if (row < 0 || row >= size)
		{
			throw std::out_of_range(fmt::format("index {} out of range; result set has {} record(s)", row, size));
		}

		if (sizeof...(R) > num_fields)
		{
			throw std::out_of_range(fmt::format("too many output fields; result set has {} fields(s)", num_fields));
		}

		return get_tuple<R...>(row);
	}

};

class phoebe_database
{
private:
	phoebe_database();
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
	
public:
	phoebe_database(const connect_parameters & params);
	~phoebe_database();
	void test_connection();		
	
	int execute_sqb(SqlBuilder & sqb);

	template <typename ... T>
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
		
	template <typename ... P>
	result_set<P...> execute_statement(const std::string & statement)
	{	
		PGresult * result_ptr = PQexecParams(meshConn, statement.c_str(),0 ,nullptr, nullptr, nullptr, nullptr, 0);
		return result_set<P...>(result_ptr);
	}

	template <typename ... P, typename ... T>
	result_set<P...> execute_proc(const std::string & procedure, const T &... t)
	{
		SqlBuilder sqb(procedure);
		build_sql(sqb, t...);
		PGresult * result_ptr = PQexecParams(meshConn, sqb.get_sql().c_str(), sqb.NumParameters(), NULL, sqb.GetParameter(), (int *)sqb.GetParamLength(), sqb.GetParamFormat(), 0);
		return result_set<P...>(result_ptr);
	}

};
