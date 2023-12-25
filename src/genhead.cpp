#include "genhead.h"
#include "lmb_sed.h"
#include <regex>

GenHead::GenHead(const std::filesystem::path& gen_path, const std::string& xml_name)
	: GenBase(gen_path, xml_name), m_need_init_count(0)
{
}

void GenHead::Replace()
{
	GenBase::Replace();

//	if (m_file_name.find("cross") != std::string::npos)
//	{
//		std::string insert_str = 
//		"\tstatic " + m_class_name + "& Instance();";
//		lmb::sed(m_gen_path.string(), 'O', "%%cross_instance%%", insert_str);
//	}
}

void GenHead::Gen0(const std::string& struct_name)
{
	GenBase::Gen0(struct_name);
	m_need_init_count = 0;
	{
		std::string insert_str = 
		"struct " + m_sub_class_name + "\n" + 
		"{\n" + 
		"\t" + m_sub_class_name + "()\n" + 
		"\t\t:%%init_struct_head%%\n" + 
		"//%%init_struct_member%%\n" + 
		"\t{}\n" + 
		"//%%struct_member%%\n" + 
		"};";
		lmb::sed(m_gen_path.string(), 'O', "%%struct_name%%", insert_str);
	}

	{
		std::string insert_str = 
		"\tint Init" + m_sub_class_name + "(PugiXmlNode RootElement);";
		lmb::sed(m_gen_path.string(), 'O', "%%initfunc_name%%", insert_str);
	}
}

void GenHead::Gen1(const std::string& member_name)
{
	GenBase::Gen1(member_name);

	CalcTypeRet ret;
	std::string member_type = this->CalcType(member_name, ret);
	if (ret.need_init)
	{
		lmb::sed(m_gen_path.string(), 's', "%%init_struct_comma%%", "");
		std::string insert_str = 
		"\t\t" + member_name + "(0),%%init_struct_comma%%";
		lmb::sed(m_gen_path.string(), 'O', "%%init_struct_member%%", insert_str);
		++m_need_init_count;
	}

	if (ret.need_declare)
	{
		std::string insert_str;
		if (ret.variable_name.empty())
		{
			insert_str = 
			"\t" + member_type + " " + member_name + ";";
		}
		else
		{
			insert_str = 
			"\t" + member_type + " " + ret.variable_name + ";";
		}
		lmb::sed(m_gen_path.string(), 'O', "%%struct_member%%", insert_str);
	}

	std::smatch sm;
	if (member_name.find("reward_item") != std::string::npos)
	{
		std::string insert_str =
		"#include \"servercommon/struct/itemlistparam.h\"";
		lmb::sed(m_gen_path.string(), 's', "%%include itemconfigdata%%", insert_str);
	}
	else if (member_name.find("weight_list") != std::string::npos
		|| member_name.find("rand_exclude") != std::string::npos
		|| member_name.find("rand") != std::string::npos
	)
	{
		std::string insert_str =
		"#include \"servercommon/utility/lmb_random.h\"";
		lmb::sed(m_gen_path.string(), 's', "%%include lmb_random%%", insert_str);
	}
	else if (std::regex_search(member_name, sm, std::regex("_\\d+parameter")))
	{
		std::string insert_str = 
		"struct " + m_sub_class_column_name + "\n" + 
		"{\n" + 
		"\t" + m_sub_class_column_name + "()\n" + 
		"\t\t:\n";
		for (int i = 0; i < m_sub_class_column_member_count - 1; ++i)
		{
			insert_str += "\t\tparam_" + std::to_string(i) + "(0),\n";
		}
		insert_str += "\t\tparam_" + std::to_string(m_sub_class_column_member_count - 1) + "(0)\n";
		insert_str += "\t{}\n";
		for (int i = 0; i < m_sub_class_column_member_count; ++i)
		{
			insert_str += "\tint param_" + std::to_string(i) + ";\n";
		}
		insert_str += "};";
		lmb::sed(m_gen_path.string(), 'O', "%%struct_column_name%%", insert_str);
	}
}
void GenHead::Gen2()
{
	GenBase::Gen2();
	if (m_need_init_count == 0)
	{
		lmb::sed(m_gen_path.string(), 'd', "%%init_struct_head%%", "");
	}
	else
	{
		lmb::sed(m_gen_path.string(), 's', "%%init_struct_head%%", "");
	}
	lmb::sed(m_gen_path.string(), 'd', "%%init_struct_member%%", "");
	lmb::sed(m_gen_path.string(), 's', ",%%init_struct_comma%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%struct_member%%", "");

	{
		std::string member_name = "\t" + this->CalcDynamicType(0) + " " + m_member_name + ";";
		lmb::sed(m_gen_path.string(), 'O', "%%member_name%%", member_name);
	}

	if (m_key_vec.size() > 0)
	{
		std::string insert_str = "\tconst " + this->CalcDynamicType(0) + "& Get" + m_sub_class_name + "Container();";
		lmb::sed(m_gen_path.string(), 'O', "%%getfunc_container%%", insert_str);
	}

	{
		std::string insert_str = 
		"\tconst " + m_sub_class_name + "%%getfunc_pointer%% Get" + m_sub_class_name + "(";
		for (size_t i = 0; i < m_key_name_vec.size(); ++i)
		{
			const auto& key = m_key_name_vec[i];
			if (i > 0)
			{
				insert_str = insert_str + ", ";
			}
			insert_str = insert_str + "int " + key;
		}
		insert_str += ");";
		lmb::sed(m_gen_path.string(), 'O', "%%getfunc_name%%", insert_str);
		if (m_key_vec.empty())
		{
			lmb::sed(m_gen_path.string(), 's', "%%getfunc_pointer%%", "&");
		}
		else
		{
			lmb::sed(m_gen_path.string(), 's', "%%getfunc_pointer%%", "*");
		}
	}
}

void GenHead::Delete()
{
	GenBase::Delete();
	lmb::sed(m_gen_path.string(), 'd', "%%struct_column_name%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%struct_name%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%member_name%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%include itemconfigdata%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%include lmb_random%%", "");

}
