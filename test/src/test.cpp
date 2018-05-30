#include <iostream>
#include <boost/version.hpp>
#include "phoebe_util/util.h"
#include "itkVersion.h"
#include "fmt/format.h"
#include "nlohmann/json.hpp"
#include <libpq-fe.h>
#include "database_io.h"
#include <optional>
#include <cstdlib>
#include <string_view>

std::tuple<int> make_tuple(int i)
{
	return std::tuple<int>(i);
}

std::tuple<double> make_tuple(double i)
{
	return std::tuple<double>(i + 0.001);
}

std::tuple<float> make_tuple(float i)
{
	return std::tuple<float>(i + 0.5);
}

template<typename T>
std::tuple<T> parse()
{
	return std::tuple<T>(make_tuple((T) 1));
}

template<typename T, typename Arg, typename... Args>
std::tuple<T, Arg, Args...> parse()
{
	return std::tuple_cat(make_tuple((T) (sizeof...(Args) + 2)), parse<Arg, Args...>());
}

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

template<typename... R, typename... P>
std::optional<std::tuple<R...>> execute_procedure(const std::string & procedure, const P &... p)
{	
	SqlBuilder sqb(procedure);
	build_sql(sqb, p...);	
	fmt::print("built sql : {}\n", sqb.get_sql());
	return std::nullopt;
}

int main()
{


	fmt::print("Testing Phoebe components.\n\n");
	fmt::print("Boost lib version: {}", BOOST_LIB_VERSION);
	itk::Version * ver = itk::Version::New();
	auto v = ver->GetITKVersion();	
	std::cout << "ITK lib version: " << v << std::endl;
	fmt::print("Format lib version {}\n", FMT_VERSION);
	fmt::print("JSON lib version {}.{}.{}\n", NLOHMANN_JSON_VERSION_MAJOR, NLOHMANN_JSON_VERSION_MINOR, NLOHMANN_JSON_VERSION_PATCH);
	fmt::print("Postgres client version {}\n", PQlibVersion());
	fmt::print("PQisthreadsafe {}\n", PQisthreadsafe());
	std::cout << std::endl;

	connect_parameters cp;
	cp.host = "phoebe.rcc.uq.edu.au";
	cp.port = "1338";
	cp.instance = "phoebe_dev";
	cp.userName = "phoebeadmin";
	cp.password = "password";

	/*
	phoebe_database db(cp);
	auto id = db.execute_procedure_json("version");
	auto [folder_id] = db.execute_proc<int>("insert_folder", "the/quick/black/fox/balh/skdhjsdf")[0];
	fmt::print("folder id: {}\n", folder_id);
	*/
	
	EXIT(0);
}