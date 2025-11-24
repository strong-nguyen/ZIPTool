#include "pch.h"

#include "ZIPReader.h"
#include "ZIPErrorCode.h"

namespace fs = std::filesystem;

void ShowHelp()
{
	std::cout << "Usage: ZIPTool.exe -x zip-file.zip" << std::endl;
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
	for (int i = 1; i < argc; ++i)
	{
		if (std::string(argv[i]) == "-x")
		{
			zip_file = argv[i + 1];
		}
	}

	fs::path zip_file_path(zip_file);
	fs::path output_dir = fs::absolute(zip_file_path).parent_path();

	ZIPReader reader(zip_file, output_dir.string());
	ZIPErrorCode ec = reader.UnZip();

	if (ec != ZIPErrorCode::Success)
	{
		std::cerr << GetZIPErrorMsg(ec) << std::endl;
		return 1;
	}

	std::cout << "Unzip success." << std::endl;
	return 0;
}