#ifndef __FILE_H__
#define __FILE_H__

#include <string>
#include <map>

struct FileData
{
	std::string full_file_name;
	time_t last_write_time = 0;
};

void get_files_time(const std::string& path_name, const std::string regex_str, std::map<std::string, FileData>& file_time_map);

typedef void(*genfilefunc)(const std::string& full_file_name, const std::string& file_name);
void compare_files_time(const std::string& xml_path, const std::string& cpp_path, genfilefunc func);

#endif
