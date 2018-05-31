#include "options.h"

Options::Options(int argc, char ** argv)
	: desc("Allowed options")
	, reg_matchHTTP("^https?:.*")	
{
	desc.add_options()
		("help,h", po::bool_switch(&help)->default_value(false), "this help message")
		("config-file,c", po::value<std::string>(&configFile), "configuration file")
		("source,s", po::value<std::string>(&data_source)->default_value(boost::filesystem::current_path().string()), "source sld file")
		("destination,d", po::value<std::string>(&data_destination)->default_value(boost::filesystem::current_path().string()), "root directory or object store location for destination data (same as source when not provided)")
		("debug", po::bool_switch(&debug)->default_value(false), "debug mode")
		("verbose,v", po::bool_switch(&verbose)->default_value(false), "print progress status")
		("database", po::bool_switch(&debug)->default_value(false), "upload to database")
		("max-time", po::value<int>(&max_time), "max time point")
		("host", po::value<std::string>(&db_parameters.host), "database host")
		("port", po::value<std::string>(&db_parameters.port), "database port")
		("instance", po::value<std::string>(&db_parameters.instance), "database instance")
		("user", po::value<std::string>(&db_parameters.userName), "database user")
		("password", po::value<std::string>(&db_parameters.password), "database user password")
	;
	try
	{	
		po::store(po::parse_command_line(argc, argv, desc), vm);
	}
	catch (po::error e)
	{
		std::cout << "Error: invalid option\n\n";
		std::cout << desc << std::endl;
		EXIT(1);
	}
	po::notify(vm);

	if (help)
	{
		std::cout << desc << std::endl;
		EXIT(0);
	}

	if (configFile.length() > 0)
	{
		ReadOptionFile(configFile, desc, vm);
	}

	po::notify(vm);
	if (vm.count("max-time"))
	{
		max_time = vm["max-time"].as<int>();
	}
	else
	{
		max_time = -1;
	}
	
	data_source = boost::filesystem::weakly_canonical(data_source).make_preferred().string();
	data_destination = boost::filesystem::weakly_canonical(data_destination).make_preferred().string();

	if (std::regex_match(data_source, reg_matchHTTP))
	{
		source_protocol = Protocol::OBJECT_STORE;
	}
	else
	{ 
		source_protocol = Protocol::FILE_SYSTEM;
	}

	if (std::regex_match(data_destination, reg_matchHTTP))
	{
		destination_protocol = Protocol::OBJECT_STORE;
	}
	else
	{
		destination_protocol = Protocol::FILE_SYSTEM;
	}
	
}

void Options::Print()
{	
	size_t width = 0;

	for (auto option : desc.options())
	{
		width = std::max(width, option.get()->canonical_display_name(0).size());
	}
	width += 2;

	width = std::max(width, std::string("Source-protocol:").size());
	width = std::max(width, std::string("Destination-protocol:").size());

	for (auto option : desc.options())
	{
		auto optionName = option.get()->canonical_display_name(0);
		auto entry = vm[optionName];
		optionName.append(":");
		if (entry.empty())
		{
			fmt::print("{:<{}} {}\n", optionName, width, "null");
		}
		else
		{
			if (auto v = boost::any_cast<std::string>(&(entry.value())))
			{
				if (optionName.find("password") != std::string::npos)
				{
					std::string password;
					password.insert(0, v->size(), '*');
					fmt::print("{:<{}} {}\n", optionName, width, password);
				}
				else
				{
					fmt::print("{:<{}} {}\n", optionName, width, *v);
				}
			}
			else if (auto v = boost::any_cast<int>(&(entry.value())))
			{
				fmt::print("{:<{}} {}\n", optionName, width, *v);
			}
			else if (auto v = boost::any_cast<bool>(&(entry.value())))
			{
				fmt::print("{:<{}} {}\n", optionName, width, *v);
			}
			else
			{
				fmt::print("{:<{}} {}\n", optionName, width, "unknown type");
			}
		}
	}
	fmt::print("{:<{}} {}\n", "Source-protocol:", width, source_protocol);
	fmt::print("{:<{}} {}\n", "Destination-protocol:", width, destination_protocol);
}

void Options::ReadOptionFile(const std::string & configFile, const po::options_description & desc, po::variables_map & vm)
{
	if (boost::filesystem::exists(configFile))
	{
		std::ifstream inputFile(configFile, std::ifstream::in);
		try
		{
			po::store(po::parse_config_file(inputFile, desc), vm);
		}
		catch (po::error e)
		{
			std::cout << "Error: invalid option in settings file" << std::endl;
			std::cout << desc << std::endl;
			EXIT(1);
		}
		vm.notify();
	}
	else
	{
		std::cout << "config file not found" << std::endl;
		EXIT(1);
	}
}

std::ostream& operator<<(std::ostream& os, const Protocol& protocol)
{
	switch (protocol)
	{
		case Protocol::FILE_SYSTEM: os << "file system"; break;
		case Protocol::OBJECT_STORE: os << "object store"; break;
	}
	return os;
}
