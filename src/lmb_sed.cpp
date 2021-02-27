#include "lmb_sed.h"
#include <regex>
#include <vector>
#include <fstream>
#include <iostream>

bool lmb::sed(const std::string& file_name, char edit_type, const std::string& regex_str, const std::string& new_str)
{
	std::ifstream ifs(file_name);
	if (!ifs.is_open())
	{
		std::cout << "open fail" << file_name << std::endl;
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
	std::regex rg(regex_str);
	std::string next_line("\n");
	for (auto& str : strs)
	{
		switch (edit_type)
		{
		case 'a':
		{
			ofs.write(str.c_str(), str.size());
			ofs.write(next_line.c_str(), next_line.size());
			std::smatch sm;
			if (std::regex_match(str, sm, rg))
			{
				ofs.write(new_str.c_str(), new_str.size());
				ofs.write(next_line.c_str(), next_line.size());
			}
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
			std::smatch sm;
			if (!std::regex_match(str, sm, rg))
			{
				ofs.write(str.c_str(), str.size());
				ofs.write(next_line.c_str(), next_line.size());
			}
		}
		break;
		}
	}
	ofs.close();
	return true;
}

