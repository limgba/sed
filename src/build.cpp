#include "build.h"
#include "genconfig.h"
#include <filesystem>
#include <chrono>
#include <regex>
#include <fstream>

int g_is_compare_file_time = 1;
void get_files_time(const std::string& path_name, const std::string regex_str, std::map<std::string, FileData>& file_time_map)
{
	namespace fs = std::filesystem;
	if (!fs::exists(path_name))
	{
		printf("path[%s] not exists\n", path_name.c_str());
		return;
	}
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
		if (!std::regex_search(extension, sm, rg))
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
			if (g_is_compare_file_time && file_time < it->second.last_write_time)
			{
				it->second = file_data;
			}
		}
	}
}

void compare_files_time(const std::string& xml_path_str, const std::string& cpp_path_str)
{
	const auto copyOptions = std::filesystem::copy_options::overwrite_existing;
	std::map<std::string, FileData> cpp_file_map;
	std::map<std::string, FileData> xml_file_map;
	get_files_time(xml_path_str, "\\.xml", xml_file_map);
	get_files_time(cpp_path_str, "\\.(cpp|h)", cpp_file_map);
	for (const auto& xml_pair : xml_file_map)
	{
		const auto& xml_name = xml_pair.first;
		const auto& xml_file_data = xml_pair.second;
		auto it = cpp_file_map.find(xml_name);
		if (cpp_file_map.end() != it && g_is_compare_file_time && xml_file_data.last_write_time <= it->second.last_write_time)
		{
			continue;
		}

		std::filesystem::path gen_path(cpp_path_str + "/" + xml_name + "/");
		std::filesystem::create_directories(gen_path);

		std::string full_cpp_path = gen_path.string() + xml_name + ".cpp";
		std::string full_head_path = gen_path.string() + xml_name + ".h";
		std::string template_cpp = xml_path_str + "/" + "template_config.cpp";
		if (!std::filesystem::exists(template_cpp))
		{
			printf("path[%s] not exists\n", template_cpp.c_str());
			return;
		}
		std::string template_head = xml_path_str + "/" + "template_config.h";
		if (!std::filesystem::exists(template_head))
		{
			printf("path[%s] not exists\n", template_head.c_str());
			return;
		}

		std::filesystem::copy(template_cpp, full_cpp_path, copyOptions);
		std::filesystem::copy(template_head, full_head_path, copyOptions);
		gen(xml_file_data.full_file_name, full_cpp_path);
		gen(xml_file_data.full_file_name, full_head_path);
	}
}
