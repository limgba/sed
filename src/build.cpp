#include "build.h"
#include "genconfig.h"
#include "genbase.h"
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
		std::string cpp_file_name = GenBase::ToFileName(xml_name);
		auto it = cpp_file_map.find(cpp_file_name);
		if (cpp_file_map.end() != it && g_is_compare_file_time && xml_file_data.last_write_time <= it->second.last_write_time)
		{
			continue;
		}

		std::filesystem::path gen_path(cpp_path_str + "/" + cpp_file_name + "/");
		std::filesystem::create_directories(gen_path);
		struct GenFileName
		{
			GenFileName(const std::string& _tf, const std::string& _gf, const std::string& _xn, bool _cc)
			{
				template_file = _tf;
				gen_file = _gf;
				xml_name = _xn;
				can_cover = _cc;
			}
			std::string template_file;
			std::string gen_file;
			std::string xml_name;
			bool can_cover;
		};
		std::vector<GenFileName> gen_file_name_vec
		{
			GenFileName(xml_path_str + "/" + "template_config.cpp", gen_path.string() + cpp_file_name + ".cpp", xml_name, true),
			GenFileName(xml_path_str + "/" + "template_config.h", gen_path.string() + cpp_file_name + ".h", xml_name, true),
			GenFileName(xml_path_str + "/" + "inherit_config.cpp", gen_path.string() + cpp_file_name + "impl.cpp", xml_name + "_impl", false),
			GenFileName(xml_path_str + "/" + "inherit_config.h", gen_path.string() + cpp_file_name + "impl.h", xml_name + "_impl", false),
		};

		for (const auto& fn : gen_file_name_vec)
		{
			if (!std::filesystem::exists(fn.template_file))
			{
				printf("path[%s] not exists\n", fn.template_file.c_str());
				return;
			}
		}
		for (const auto& fn : gen_file_name_vec)
		{
			if (!fn.can_cover && std::filesystem::exists(fn.gen_file))
			{
				continue;
			}
			std::filesystem::copy(fn.template_file, fn.gen_file, copyOptions);
			gen(xml_file_data.full_file_name, fn.gen_file, fn.xml_name);
		}
	}
}

