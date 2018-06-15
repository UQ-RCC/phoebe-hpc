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

#include "curlpp/cURLpp.hpp"
#include "curlpp/Easy.hpp"
#include "curlpp/Options.hpp"
#include "curlpp/Exception.hpp"

#include <vector>;


namespace curlpp::FormParts
{
	class Buffer : public FormPart
	{
	public:
		Buffer(std::string name, const char * buffer, long length)
			: FormPart(name)
			, buffer(buffer)
			, length(length)
		{}
		~Buffer()
		{}
		Buffer * Buffer::clone() const
		{
			return new Buffer(* this);
		}

		void Buffer::add(::curl_httppost ** first, ::curl_httppost ** last)
		{
			curl_formadd(first,
				last,
				CURLFORM_PTRNAME,
				mName.c_str(),
				CURLFORM_BUFFER,
				mName.c_str(),
				CURLFORM_BUFFERPTR,
				buffer,
				CURLFORM_BUFFERLENGTH,
				length,
				CURLFORM_END);
		}

	private:		
		const char * buffer;
		long length;
	};
}


int main()
{
	
	//curlpp::initialize(CURL_GLOBAL_ALL);

	fmt::print("Testing Phoebe components.\n\n");
	fmt::print("Boost lib version: {}", BOOST_LIB_VERSION);
	itk::Version * ver = itk::Version::New();
	auto v = ver->GetITKVersion();	
	std::cout << "ITK lib version: " << v << std::endl;
	fmt::print("Format lib version {}\n", FMT_VERSION);
	fmt::print("JSON lib version {}.{}.{}\n", NLOHMANN_JSON_VERSION_MAJOR, NLOHMANN_JSON_VERSION_MINOR, NLOHMANN_JSON_VERSION_PATCH);
	fmt::print("Postgres client version {}\n", PQlibVersion());
	//fmt::print("libCurlpp version {}\n", LIBCURLPP_VERSION);

	std::cout << std::endl;

	connect_parameters cp;
	cp.host = "phoebe.rcc.uq.edu.au";
	cp.port = "1339";
	cp.instance = "phoebe_dev";
	cp.userName = "phoebeadmin";
	cp.password = "password";

	//curlpp::Easy handle;
	//handle.setOpt(curlpp::Options::Url("http://localhost:1337/0a14101f-fe86-4d9a-8530-8b8e6d99055f"));
	//auto out = curlpp::options::Url("http://example.com");
	//std::cout << out << std::endl;
	
	//handle.setOpt(curlpp::Options::Url("http://example.com"));
	//handle.perform();
	
	

	/*
	phoebe_database db(cp);
	auto id = db.execute_procedure_json("version");
	auto [folder_id] = db.execute_proc<int>("insert_folder", "the/quick/black/fox/balh/skdhjsdf")[0];
	fmt::print("folder id: {}\n", folder_id);
	*/
	

	std::string b = "this is it...";
	std::string a = "and some more this is it...";

	try {


		curlpp::Cleanup cleaner;
		curlpp::Easy request;

		request.setOpt(new curlpp::options::Url("http://localhost:1337"));
		//request.setOpt(new curlpp::options::Verbose(true)); 

		{
			// Forms takes ownership of pointers!
			curlpp::Forms formParts;
			formParts.push_back(new curlpp::FormParts::Content("name1", "value1"));
			formParts.push_back(new curlpp::FormParts::Content("name2", "value2"));
			formParts.push_back(new curlpp::FormParts::Content("name3", "and some more"));
			//formParts.push_back(new curlpp::FormParts::File("file", "d:/data/test.buf"));
			formParts.push_back(new curlpp::FormParts::Buffer("my buffer", b.c_str(), b.length()));
			formParts.push_back(new curlpp::FormParts::Buffer("more buffer", a.c_str(), a.length()));			
			request.setOpt(new curlpp::options::HttpPost(formParts));
		}

		request.perform();
	}
	catch (curlpp::LogicError & e) {
		std::cout << e.what() << std::endl;
	}
	catch (curlpp::RuntimeError & e) {
		std::cout << e.what() << std::endl;
	}

	curlpp::terminate();
	EXIT(0);
}
