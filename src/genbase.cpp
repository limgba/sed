#include "genbase.h"
#include "lmb_sed.h"
#include <algorithm>
#include <regex>

GenBase::GenBase(const std::filesystem::path& gen_path)
	: m_gen_path(gen_path), m_member_count(0)
{
	m_def_name = this->ToDefName(m_gen_path.stem().string());
	m_file_name = this->ToFileName(m_gen_path.stem().string());
	m_class_name = this->ToClassName(m_gen_path.stem().string()) + "Config";
}

void GenBase::Replace()
{
	lmb::sed(m_gen_path.string(), 's', "%%def_name%%", m_def_name);
	lmb::sed(m_gen_path.string(), 's', "%%class_name%%", m_class_name);
	lmb::sed(m_gen_path.string(), 's', "%%file_name%%", m_file_name);
}


std::string GenBase::ToDefName(const std::string& in_str)
{
	std::string out_str = in_str;
	std::transform(out_str.begin(), out_str.end(), out_str.begin(), [](unsigned char c) { return std::toupper(c); });
	return out_str;
}

std::string GenBase::ToFileName(const std::string& in_str)
{
	std::string out_str = in_str;
	std::transform(out_str.begin(), out_str.end(), out_str.begin(), [](unsigned char c) { return std::tolower(c); });
	return out_str;
}

std::string GenBase::ToClassName(const std::string& in_str)
{
	std::string out_str = in_str;
	unsigned char last_c = 0;
	std::transform(out_str.begin(), out_str.end(), out_str.begin(), [&](unsigned char c)->unsigned char {
		if (last_c == 0 || last_c == '_')
		{
			last_c = c;
			return std::toupper(c); 
		}
		last_c = c;
		return c;
	});
	auto it = std::remove(out_str.begin(), out_str.end(), '_');
	out_str.erase(it, out_str.end());
	return out_str;
}

std::string GenBase::CalcType(const std::string& in_str, CalcTypeRet& ret)
{
	std::smatch sm;
	std::string out_str;
	ret.variable_name = in_str;

	if (std::regex_search(in_str, sm, std::regex("hp|gongji|fangyu")))
	{
		out_str = "Attribute";
	}
	else if (std::regex_search(in_str, sm, std::regex("reward_*item")))
	{
		ret.need_init = false;
		out_str = "std::vector<ItemConfigData>";
	}
	else if (std::regex_search(in_str, sm, std::regex("drop")))
	{
		ret.need_init = false;
		out_str = "std::vector<UInt16>";
	}
	else if (std::regex_search(in_str, sm, std::regex("area")))
	{
		ret.need_init = false;
		out_str = "PointConfig";
	}
	else if (std::regex_search(in_str, sm, std::regex("item_*id|stuff_*id|equip_*id")))
	{
		out_str = "ItemID";
	}
	else if (std::regex_search(in_str, sm, std::regex("capability|money")))
	{
		out_str = "long long";
	}
	else if (std::regex_search(in_str, sm, std::regex("attr_type_\\d|attr_value_\\d")))
	{
		ret.need_init = false;
		if (sm.size() == 0 || sm[0] != "attr_type_0")
		{
			ret.need_declare = false;
		}
		else
		{
			ret.variable_name = "attr_map";
		}
		out_str = "std::map<int, Attribute>";
	}
	else
	{
		out_str = "int";
	}
	return out_str;
}

std::string GenBase::CalcDynamicType(size_t index)
{
	std::string ret_str;
	for (size_t i = index; i < m_key_vec.size(); ++i)
	{
		ret_str += m_key_vec[i];
	}
	ret_str = ret_str + m_sub_class_name + std::string(m_key_vec.size() - index, '>');
	return ret_str;
}

void GenBase::Gen0(const std::string& struct_name)
{
	const std::string hump_struct_name = this->ToClassName(struct_name);
	m_sub_class_name = m_class_name + hump_struct_name + "Cfg";
	m_member_name = "m_" + this->ToFileName(struct_name) + "_cfg_container";
	m_key_vec.clear();
	m_key_name_vec.clear();
	m_member_count = 0;
}
void GenBase::Gen1(const std::string& member_name)
{
	std::smatch sm;
	if (std::regex_search(member_name, sm, std::regex("index")))
	{
		m_key_vec.push_back("std::vector<");
		m_key_name_vec.push_back(member_name);
	}
	else if (std::regex_search(member_name, sm, std::regex("key|_range|_rrange")))
	{
		m_key_vec.push_back("std::map<int, ");
		m_key_name_vec.push_back(member_name);
	}
	++m_member_count;
}
void GenBase::Gen2() {}

void GenBase::Delete()
{
	lmb::sed(m_gen_path.string(), 'd', "%%getfunc_name%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%getfunc_container%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%initfunc_name%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%cross_instance%%", "");
}
