#include <iostream>
#include <boost/version.hpp>
#include "phoebe_util/util.h"
#include "itkVersion.h"
#include "fmt/format.h"
#include "nlohmann/json.hpp"
#include <libpq-fe.h>
#include "database_lib.h"

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

	DBExecutor executor(cp);
	
	auto id = executor.execute_procedure("version");
	std::cout << "query retured: " << id << std::endl;
	
	auto json = executor.execute_procedure_json("version");
	std::cout << "query retured: " << json.dump(4) << std::endl;

	EXIT(0);
}