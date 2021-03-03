#include "lmb_sed.h"
#include "build.h"
#include <iostream>

int main(int argc, char* argv[])
{
	compare_files_time("./config", "./tmp", [](const std::string& full_file_name, const std::string& file_name){std::cout << full_file_name << " " << file_name << std::endl;});
	return 0;
}
