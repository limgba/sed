#include "lmb_sed.h"
#include "build.h"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("need 2 arg, xml_path and cpp_path");
		return 1;
	}
	if (argc == 4)
	{
		g_is_compare_file_time = atoi(argv[3]);
	}
	compare_files_time(argv[1], argv[2]);
	return 0;
}
