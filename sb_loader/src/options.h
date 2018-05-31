#pragma once

#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include "fmt/format.h"
#include "fmt/ostream.h"
#include "phoebe_util/util.h"
#include "database_io.h"

namespace po = boost::program_options;

enum class Protocol {FILE_SYSTEM, OBJECT_STORE};
std::ostream& operator<<(std::ostream& os, const Protocol& protocol);

class Options
{
public:
	bool help;
	bool debug;
	bool database;
	std::string data_source;
	std::string data_destination;
	Protocol source_protocol;
	Protocol destination_protocol;
	bool original_filename;
	std::string configFile;	
	int max_time;
	bool verbose;
	connect_parameters db_parameters;
	Options(int argc, char ** argv);
	void Print();
private:
	std::string file_stem;
	po::options_description desc;
	po::variables_map vm;
	const std::regex reg_matchHTTP;
	void ReadOptionFile(const std::string & configFile, const po::options_description & desc, po::variables_map & vm);	
};