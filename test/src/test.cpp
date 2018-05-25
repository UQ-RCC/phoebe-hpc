#include <iostream>
#include <boost/version.hpp>
#include "phoebe_util/util.h"
#include "itkVersion.h"
#include "fmt/format.h"
#include "nlohmann/json.hpp"
#include <libpq-fe.h>
#include "database_io.h"
#include <optional>

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
	std::cout << std::endl;

	ConnectParameters cp;
	cp.host = "phoebe.rcc.uq.edu.au";
	cp.port = "1338";
	cp.instance = "phoebe_dev";
	cp.userName = "phoebeadmin";
	cp.password = "password";

	std::optional<std::tuple<int, int, int>> o;

	if (!o)
	{
		fmt::print("nothing in o\n");
	}

	o.emplace(std::make_tuple(1, 2, 3));

	if (o)
	{
		auto[f0, f1, f2] = *o;		
		fmt::print("optional : {} {} {}\n", f0, f1, f2);
	}
	
	PhoebeDatabase db(cp);
	auto id = db.execute_procedure_json("version");
	std::cout << "query retured: " << id << std::endl;

	auto[a, b, c] = parse<int, float, double>();
	fmt::print("{} {} {}\n", a, b, c);

	auto [f] = parse<float>();
	fmt::print("{}\n", f);


	EXIT(0);
}