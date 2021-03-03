#include "build.h"
#include <filesystem>
#include <chrono>
#include <regex>


void get_files_time(const std::string& path_name, const std::string regex_str, std::map<std::string, FileData>& file_time_map)
{
	namespace fs = std::filesystem;
	time_t fs_now_time = std::chrono::duration_cast<std::chrono::seconds>(fs::file_time_type::clock::now().time_since_epoch()).count();
	time_t system_now_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	time_t elapse = fs_now_time - system_now_time;
	std::regex rg(regex_str);
	std::smatch sm;
	for (auto& p : fs::recursive_directory_iterator(path_name))
	{
		if (!p.is_regular_file())
		{
			continue;
		}
		std::string extension = p.path().extension().string();
		if (!std::regex_match(extension, sm, rg))
		{
			continue;
		}
		std::string full_file_name = p.path().string();
		std::string stem = p.path().stem().string();
		time_t file_time = std::chrono::duration_cast<std::chrono::seconds>(p.last_write_time().time_since_epoch()).count() - elapse;
		FileData file_data;
		file_data.full_file_name = full_file_name;
		file_data.last_write_time = file_time;
		auto it = file_time_map.find(stem);
		if (it == file_time_map.end())
		{
			file_time_map.emplace(stem, file_data);
		}
		else
		{
			if (file_time < it->second.last_write_time)
			{
				it->second = file_data;
			}
		}
	}
}

void compare_files_time(const std::string& xml_path, const std::string& cpp_path, genfilefunc func)
{
	std::map<std::string, FileData> cpp_file_map;
	std::map<std::string, FileData> xml_file_map;
	get_files_time(xml_path, "\\.xml", xml_file_map);
	get_files_time(cpp_path, "\\.(cpp|h)", cpp_file_map);
	for (const auto& xml_pair : xml_file_map)
	{
		const auto& xml_name = xml_pair.first;
		const auto& xml_file_data = xml_pair.second;
		auto it = cpp_file_map.find(xml_name);
		if (cpp_file_map.end() == it)
		{
			std::filesystem::copy_file(xml_file_data.full_file_name, cpp_path + "/" + xml_name + ".cpp");
			std::filesystem::copy_file(xml_file_data.full_file_name, cpp_path + "/" + xml_name + ".h");
			func(xml_file_data.full_file_name, xml_name);
		}
		else
		{
			if (xml_file_data.last_write_time <= it->second.last_write_time)
			{
				continue;
			}
			func(xml_file_data.full_file_name, xml_name);
		}
	}
}
