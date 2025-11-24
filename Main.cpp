#include "pch.h"

#include "ZIPReader.h"
#include "ZIPErrorCode.h"

namespace fs = std::filesystem;

void ShowHelp()
{
	std::cout << "Usage: ZIPTool.exe -x zip-file.zip [-o output-path/]" << std::endl;
}

int main(int argc, char** argv)
{
	if (argc == 1 || 
		(argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")))
	{
		ShowHelp();
		return 0;
	}

	std::string zip_file;
	std::string output_path;
	for (int i = 1; i < argc; ++i)
	{
		if (std::string(argv[i]) == "-x" && i + 1 < argc)
		{
			zip_file = argv[i + 1];
		}
		else if (std::string(argv[i]) == "-o" && i + 1 < argc)
		{
			output_path = argv[i + 1];
		}
	}

	fs::path zip_file_path(zip_file);
	fs::path output_dir(output_path);
	if (output_dir.empty())  // If user not specify output path, ZIPTool will extract to the same dir as zip file
	{
		fs::path output_dir = fs::absolute(zip_file_path).parent_path();
	}
	else
	{
		if (!output_dir.is_absolute())  // Input path is relative
		{
			output_dir = fs::current_path() / output_dir;
		}
	}

	ZIPReader reader(zip_file, output_dir.string());
	ZIPErrorCode ec = reader.UnZip();

	if (ec != ZIPErrorCode::Success)
	{
		std::cerr << std::format("Unzip failed, error: {}.", GetZIPErrorMsg(ec)) << std::endl;
		return 1;
	}
	else
	{
		std::cout << std::format("Unzip successsfully to {}.", output_dir.string()) << std::endl;
	}

	std::cout << "Unzip success." << std::endl;
	return 0;
}