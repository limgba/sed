#include "lmb_sed.h"
#include <regex>
#include <vector>
#include <fstream>
#include <iostream>

bool lmb::sed(const std::string& file_name, char edit_type, const std::string& regex_str, const std::string& new_str)
{
	std::vector<char> edit_type_vec
	{
		'a',
		'O',
		'i',
		's',
		'd',
		'm',
	};
	auto vec_it = std::find(edit_type_vec.begin(), edit_type_vec.end(), edit_type);
	if (edit_type_vec.end() == vec_it)
	{
		std::cout << "edit_type error" << std::endl;
		return false;
	}
	std::ifstream ifs(file_name);
	if (!ifs.is_open())
	{
		std::cout << "ifs open fail" << file_name << std::endl;
		return false;
	}
	std::vector<std::string> strs;
	std::string line_str;
	while (std::getline(ifs, line_str))
	{
		strs.push_back(line_str);
	}
	ifs.close();
	std::ofstream ofs(file_name, std::ios::out | std::ios::trunc);
	if (!ofs.is_open())
	{
		std::cout << "ofs open fail" << file_name << std::endl;
		return false;
	}
	bool find_m = false;
	std::regex rg(regex_str);
	std::smatch sm;
	std::string next_line("\n");
	for (auto& str : strs)
	{
		switch (edit_type)
		{
		case 'a':
		{
			ofs.write(str.c_str(), str.size());
			ofs.write(next_line.c_str(), next_line.size());
			if (std::regex_search(str, sm, rg))
			{
				ofs.write(new_str.c_str(), new_str.size());
				ofs.write(next_line.c_str(), next_line.size());
			}
		}
		break;
		case 'O':
		{
			if (std::regex_search(str, sm, rg))
			{
				ofs.write(new_str.c_str(), new_str.size());
				ofs.write(next_line.c_str(), next_line.size());
			}
			ofs.write(str.c_str(), str.size());
			ofs.write(next_line.c_str(), next_line.size());
		}
		break;
		case 'i':
		{
			std::string insert_str = "$&" + new_str;
			str = std::regex_replace(str, rg, insert_str);
			ofs.write(str.c_str(), str.size());
			ofs.write(next_line.c_str(), next_line.size());
		}
		break;
		case 's':
		{
			str = std::regex_replace(str, rg, new_str);
			ofs.write(str.c_str(), str.size());
			ofs.write(next_line.c_str(), next_line.size());
		}
		break;
		case 'd':
		{
			if (!std::regex_search(str, sm, rg))
			{
				ofs.write(str.c_str(), str.size());
				ofs.write(next_line.c_str(), next_line.size());
			}
		}
		break;
		case 'm':
		{
			if (!find_m && std::regex_search(str, sm, rg))
			{
				find_m = true;
			}
			ofs.write(str.c_str(), str.size());
			ofs.write(next_line.c_str(), next_line.size());
		}
		break;
		default:
		{
			ofs.write(str.c_str(), str.size());
			ofs.write(next_line.c_str(), next_line.size());
		}
		break;
		}
	}
	ofs.close();
	if (edit_type == 'm')
	{
		return find_m;
	}
	return true;
}

