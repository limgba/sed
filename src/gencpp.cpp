#include "gencpp.h"
#include "lmb_sed.h"
#include <regex>
#include <stdio.h>

GenCpp::GenCpp(const std::filesystem::path& gen_path, const std::string& xml_name)
	: GenBase(gen_path, xml_name)
{
}

void GenCpp::Replace()
{
	GenBase::Replace();

//	if (m_file_name.find("cross") != std::string::npos)
//	{
//		std::string insert_str = 
//		m_class_name + "& " + m_class_name + "::Instance()\n" +
//		"{\n" +
//		"\tstatic " + m_class_name + "instance;\n" +
//		"\treturn instance;\n" +
//		"}";
//		lmb::sed(m_gen_path.string(), 'O', "%%cross_instance%%", insert_str);
//	}
}

void GenCpp::Gen0(const std::string& struct_name)
{
	GenBase::Gen0(struct_name);

	{
		std::string insert_str = 
		"\tPUGI_XML_LOAD_CONFIG(\"" + struct_name + "\", Init" + m_sub_class_name + ");";
		lmb::sed(m_gen_path.string(), 'O', "%%load_config%%", insert_str);
	}

	{
		std::string insert_str = 
		"const " + m_sub_class_name + "%%getfunc_pointer%% " + m_class_name + "::Get" + m_sub_class_name + "(%%getfunc_args%%)\n" +
		"{\n" +
		"//%%getfunc_content%%\n" +
		"}";
		lmb::sed(m_gen_path.string(), 'O', "%%getfunc_name%%", insert_str);
	}

	{
		std::string insert_str = 
		"int " + m_class_name + "::Init" + m_sub_class_name + "(PugiXmlNode RootElement)\n" + 
		"{\n" + 
		"\tdecltype(" + m_member_name + ") tmp_container;\n" + 
		"\tPugiXmlNode dataElement = RootElement.child(\"data\");\n" + 
		"\twhile (!dataElement.empty())\n" + 
		"\t{\n" + 
		"//%%initfunc_def_cfg%%\n" +
		"//%%initfunc_content%%\n" +
		"\t}\n" +
		"%%initfunc_end%%\n" +
		"\treturn 0;\n" +
		"}";
		lmb::sed(m_gen_path.string(), 'O', "%%initfunc_name%%", insert_str);
	}
}

void GenCpp::Gen1(const std::string& member_name)
{
	GenBase::Gen1(member_name);
	std::smatch sm;

	const std::string member_count_str = std::to_string(m_member_count);

	if (std::regex_search(member_name, sm, std::regex("reward_*item")))
	{
		{
			std::string read_str = member_name;
			if (read_str.find_last_of("_list") != std::string::npos)
			{
				read_str = read_str.substr(0, read_str.size() - 5);
			}
			std::string insert_str;
//			if (m_file_name.find("cross") != std::string::npos)
//			{
//				insert_str += "\t\tint " + member_name + "_ret = ItemConfigData::ReadConfigListNoCheck(dataElement, \"" + read_str + "\", cfg." + member_name + ");\n";
//			}
//			else
			{
				insert_str += "\t\tint " + member_name + "_ret = ItemConfigData::ReadConfigList(dataElement, \"" + read_str + "\", cfg." + member_name + ");\n";
				{
					std::string insert_str = 
					"#include \"item/itempool.h\"";
					lmb::sed(m_gen_path.string(), 's', "%%include itempool%%", insert_str);
				}
			}
			insert_str = insert_str + 
			"\t\tif (" + member_name + "_ret < 0)\n" + 
			"\t\t{\n" + 
			"\t\t\treturn -" + member_count_str + "000 + " + member_name + "_ret;\n" + 
			"\t\t}\n";
			lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
		}
	}
	else if (std::regex_search(member_name, sm, std::regex("drop")))
	{
		{
			std::string insert_str = 
			"\t\tint " + member_name + "_ret = DROPPOOL->ReadDropConfig(dataElement, \"" + member_name + "\", cfg." + member_name + ");\n" + 
			"\t\tif (" + member_name + "_ret < 0)\n" + 
			"\t\t{\n" + 
			"\t\t\treturn -" + member_count_str + "000 + " + member_name + "_ret;\n" + 
			"\t\t}\n";
			lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
		}

		{
			std::string insert_str = 
			"#include \"monster/drop/droppool.hpp\"";
			lmb::sed(m_gen_path.string(), 's', "%%include droppool%%", insert_str);
		}
	}
	else if (std::regex_search(member_name, sm, std::regex("area")))
	{
		{
			std::string insert_str = 
			"\t\tbool " + member_name + "_ret = cfg." + member_name + ".ReadConfig(dataElement, \"" + member_name + "\");\n" +
			"\t\tif (!" + member_name + "_ret)\n" + 
			"\t\t{\n" +
			"\t\t\treturn -" + member_count_str + "000;\n" + 
			"\t\t}\n";
			lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
		}
	}
	else if (std::regex_search(member_name, sm, std::regex("item_*id|stuff_*id|equip_*id")))
	{
		{
			std::string insert_str;
//			if (m_file_name.find("cross") != std::string::npos)
//			{
//				insert_str = insert_str + "\t\tif (!PugiGetSubNodeValue(dataElement, \"" + member_name + "\", cfg." + member_name + "))\n";
//			}
//			else
			{
				insert_str = insert_str + "\t\tif (!PugiGetSubNodeValue(dataElement, \"" + member_name + "\", cfg." + member_name + ") || nullptr == ITEMPOOL->GetItem(cfg." + member_name + "))\n";
				{
					std::string insert_str = 
					"#include \"item/itempool.h\"";
					lmb::sed(m_gen_path.string(), 's', "%%include itempool%%", insert_str);
				}
			}
		
			insert_str = insert_str + "\t\t{\n" + 
			"\t\t\treturn -" + member_count_str + ";\n" + 
			"\t\t}\n";
			lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
		}

	}
	else if (std::regex_search(member_name, sm, std::regex("attr_type_\\d|attr_value_\\d")))
	{
		if (member_name == "attr_type_0")
		{
			std::string insert_str =
			"\t\tReadAttrTypeValueConfig config_tool;\n";
			insert_str = insert_str + "\t\tint " + member_name + "_ret = config_tool.Read(dataElement, \"attr\", cfg.attr_map);\n" + 
			"\t\tif (" + member_name + "_ret < 0)\n" +
			"\t\t{\n" + 
			"\t\t\treturn -" + member_count_str + "000 + " + member_name + "_ret;\n" + 
			"\t\t}\n";
			lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);

			{
				std::string insert_str = 
				"#include \"obj/character/attribute.hpp\"";
				lmb::sed(m_gen_path.string(), 's', "%%include attribute%%", insert_str);
			}
		}

	}
	else if (std::regex_search(member_name, sm, std::regex("weight_list")))
	{
		{
			std::string insert_str = "\t\tstd::string " + member_name + "_str;\n";
			insert_str += 
			"\t\tif (!PugiGetSubNodeValue(dataElement, \"" + member_name + "\", " + member_name + "_str) || " + member_name + "_str.empty())\n" + 
			"\t\t{\n" + 
			"\t\t\treturn -" + member_count_str + ";\n" + 
			"\t\t}\n";
			insert_str += "\t\tstd::vector<int>" + member_name + "_vec = SplitStringInt(" + member_name + "_str, \",\");\n";
			insert_str += "\t\tcfg." + member_name + ".push_back(" + member_name + "_vec);\n";
			lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
		}
	}
	else if (std::regex_search(member_name, sm, std::regex("str")))
	{
		{
			std::string insert_str = 
			"\t\tif (!PugiGetSubNodeValue(dataElement, \"" + member_name + "\", cfg." + member_name + "))\n" + 
			"\t\t{\n" + 
			"\t\t\treturn -" + member_count_str + ";\n" + 
			"\t\t}\n";
			lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
		}
	}
	else if (std::regex_search(member_name, sm, std::regex("(_comma_pipe)|(_pipe_comma)|(_comma)|(_pipe)")))
	{
		{
			std::string insert_str;
			if (sm[0] == "_comma")
			{
				insert_str += "\t\tint " + member_name + "_ret = this->ReadList(dataElement, \"" + member_name + "\", cfg." + member_name + ", \",\");\n";
			}
			else if (sm[0] == "_pipe")
			{
				insert_str += "\t\tint " + member_name + "_ret = this->ReadList(dataElement, \"" + member_name + "\", cfg." + member_name + ", \"|\");\n";
			}
			else
			{
				if (sm[1] == "_comma_pipe")
				{
					insert_str += "\t\tint " + member_name + "_ret = this->ReadListInList(dataElement, \"" + member_name + "\", cfg." + member_name + ", \",\", \"|\");\n";
				}
				else if (sm[2] == "_pipe_comma")
				{
					insert_str += "\t\tint " + member_name + "_ret = this->ReadListInList(dataElement, \"" + member_name + "\", cfg." + member_name + ", \"|\", \",\");\n";
				}
			}
			insert_str += "\t\tif (" + member_name + "_ret < 0)\n" + 
			"\t\t{\n" + 
			"\t\t\treturn -" + member_count_str + "000 + " + member_name + "_ret;\n" + 
			"\t\t}\n";
			lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
		}
	}
	else if (std::regex_search(member_name, sm, std::regex("negative")))
	{
		{
			std::string insert_str = 
			"\t\tif (!PugiGetSubNodeValue(dataElement, \"" + member_name + "\", cfg." + member_name + "))\n" + 
			"\t\t{\n" + 
			"\t\t\treturn -" + member_count_str + ";\n" + 
			"\t\t}\n";
			lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
		}
	}
	else if (std::regex_search(member_name, sm, std::regex("_\\d+parameter_list")))
	{
		{
			std::string insert_str = 
			"\t\t{\n"
			"\t\t\tstd::string " + member_name + "_str;\n" +
			"\t\t\tPugiGetSubNodeValue(dataElement, \"" + member_name + "\", " + member_name + "_str);\n" +
			"\t\t\tstd::vector<std::string> " + member_name + "_vec = SplitString(" + member_name + "_str, \",\");\n" +
			"\t\t\tfor (const std::string& " + member_name + " : " + member_name + "_vec)\n" +
			"\t\t\t{\n" +
			"\t\t\t\t" + m_sub_class_column_name + " " + ToFileName(m_sub_class_column_name) + ";\n" +
			"\t\t\t\tstd::vector<int> " + member_name + "_sub_vec = SplitStringInt(" + member_name + ", \":\");\n" +
			"\t\t\t\tif ((int)" + member_name + "_sub_vec.size() != " + std::to_string(m_sub_class_column_member_count) + ")\n" +
			"\t\t\t\t{\n" +
			"\t\t\t\t\treturn -" + member_count_str + ";\n" + 
			"\t\t\t\t}\n";
			for (int i = 0; i < m_sub_class_column_member_count; ++i)
			{
				insert_str += "\t\t\t\t" + ToFileName(m_sub_class_column_name) + ".param_" + std::to_string(i) + " = " + member_name + "_sub_vec[" + std::to_string(i) + "];\n";
			}
			insert_str += "\t\t\t\tcfg." + member_name + ".push_back(" + ToFileName(m_sub_class_column_name) + ");\n" +
			"\t\t\t}\n" +
			"\t\t}\n";
			lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
		}
	}
	else if (std::regex_search(member_name, sm, std::regex("_\\d+parameter")))
	{
		{
			std::string insert_str = 
			"\t\t{\n"
			"\t\t\tstd::string " + member_name + "_str;\n" +
			"\t\t\tPugiGetSubNodeValue(dataElement, \"" + member_name + "\", " + member_name + "_str);\n" +
			"\t\t\tstd::vector<int> " + member_name + "_vec = SplitStringInt(" + member_name + "_str, \":\");\n" +
			"\t\t\tif ((int)" + member_name + "_vec.size() != " + std::to_string(m_sub_class_column_member_count) + ")\n" +
			"\t\t\t{\n" +
			"\t\t\t\treturn -" + member_count_str + ";\n" + 
			"\t\t\t}\n";
			for (int i = 0; i < m_sub_class_column_member_count; ++i)
			{
				insert_str += "\t\t\tcfg." + member_name + ".param_" + std::to_string(i) + " = " + member_name + "_vec[" + std::to_string(i) + "];\n";
			}
			insert_str += "\t\t}\n";
			lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
		}
	}
	else
	{
		{
			std::string insert_str = 
			"\t\tif (!PugiGetSubNodeValue(dataElement, \"" + member_name + "\", cfg." + member_name + ") || cfg." + member_name + " < 0)\n" + 
			"\t\t{\n" + 
			"\t\t\treturn -" + member_count_str + ";\n" + 
			"\t\t}\n";
			lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
		}
	}

}

void GenCpp::Gen2()
{
	GenBase::Gen2();
	std::smatch sm;
	if (m_member_count > 0)
	{
		std::string insert_str = 
		"\t\t" + m_sub_class_name + " cfg;";
		lmb::sed(m_gen_path.string(), 'O', "%%initfunc_def_cfg%%", insert_str);
	}
	if (!m_key_name_vec.empty())
	{
		std::string insert_str = 
		"\t\tauto& container_0 = tmp_container;";
		lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
	}

	if (!m_key_vec.empty())
	{
		std::string insert_str = 
		"\tauto& container_0 = " + m_member_name + ";";
		lmb::sed(m_gen_path.string(), 'O', "%%getfunc_content%%", insert_str);
	}
	
	for (size_t i = 0; i < m_key_vec.size() && i < m_key_name_vec.size(); ++i)
	{
		const size_t count = i + 1;
		const std::string count_str = std::to_string(count);
		const std::string i_str = std::to_string(i);
		const std::string& key_str = m_key_vec[i];
		const std::string& key_name_str = m_key_name_vec[i];
		if (key_str.find("vector") != std::string::npos)
		{
			{
				std::string insert_str = 
				"\tif (" + key_name_str + " < 0 || (size_t)" + key_name_str + " >= container_" + i_str + ".size())\n" + 
				"\t{\n" + 
				"\t\treturn nullptr;\n" + 
				"\t}\n" +
				"\tauto& container_" + count_str + " = container_" + i_str + "[" + key_name_str + "];\n";
				lmb::sed(m_gen_path.string(), 'O', "%%getfunc_content%%", insert_str);
			}

			{
				std::string insert_str = 
				"\t\tif ((size_t)cfg." + key_name_str + " > container_" + i_str + ".size())\n" + 
				"\t\t{\n" + 
				"\t\t\treturn -" + count_str + "0000;\n" + 
				"\t\t}\n" + 
				"\t\telse if ((size_t)cfg." + key_name_str + " == container_" + i_str + ".size())\n" + 
				"\t\t{\n" + 
				"\t\t\t" + this->CalcDynamicType(i + 1) + " tmp;\n" + 
				"\t\t\tcontainer_" + i_str + ".push_back(tmp);\n" +
				"\t\t}\n" + 
				"\t\tif ((size_t)cfg." + key_name_str + " >= container_" + i_str + ".size())\n" + 
				"\t\t{\n" + 
				"\t\t\treturn -" + count_str + "0001;\n" + 
				"\t\t}\n" +
				"\t\tauto& container_" + count_str + " = container_" + i_str + "[cfg." + key_name_str + "];\n";
				lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
			}
		}
		else if (key_str.find("map") != std::string::npos)
		{
			if (std::regex_search(key_name_str, sm, std::regex("key")))
			{
				std::string insert_str = 
				"\tauto map_it_" + i_str + " = container_" + i_str + ".find(" + key_name_str + ");\n" + 
				"\tif (map_it_" + i_str + " == container_" + i_str + ".end())\n" + 
				"\t{\n" + 
				"\t\treturn nullptr;\n" + 
				"\t}\n" +
				"\tauto& container_" + count_str + " = map_it_" + i_str + "->second;\n";
				lmb::sed(m_gen_path.string(), 'O', "%%getfunc_content%%", insert_str);
			}
			else if (std::regex_search(key_name_str, sm, std::regex("_range")))
			{
				{
					std::string insert_str = 
					"\tauto map_it_" + i_str + " = std::lower_bound(container_" + i_str + ".rbegin(), container_" + i_str + ".rend(), " + key_name_str + ", \n" + 
					"\t\t[](const " + this->MapToPair(this->CalcDynamicType(i)) + "& element, int value)\n"
					"\t\t{\n"
					"\t\t\treturn element.first > value;\n"
					"\t\t});\n"
					"\tif (map_it_" + i_str +" == container_" + i_str + ".rend())\n"
					"\t{\n"
					"\t\treturn nullptr;\n"
					"\t}\n"
					"\tauto& container_" + count_str + " = map_it_" + i_str + "->second;\n";
					lmb::sed(m_gen_path.string(), 'O', "%%getfunc_content%%", insert_str);
				}
			}
			else if (std::regex_search(key_name_str, sm, std::regex("_rrange")))
			{
				{
					std::string insert_str = 
					"\tauto map_it_" + i_str + " = std::lower_bound(container_" + i_str + ".begin(), container_" + i_str + ".end(), " + key_name_str + ", \n" + 
					"\t\t[](const " + this->MapToPair(this->CalcDynamicType(i)) + "& element, int value)\n"
					"\t\t{\n"
					"\t\t\treturn element.first < value;\n"
					"\t\t});\n"
					"\tif (map_it_" + i_str +" == container_" + i_str + ".end())\n"
					"\t{\n"
					"\t\treturn nullptr;\n"
					"\t}\n"
					"\tauto& container_" + count_str + " = map_it_" + i_str + "->second;\n";
					lmb::sed(m_gen_path.string(), 'O', "%%getfunc_content%%", insert_str);
				}
			}

			{
				std::string insert_str = 
				"\t\tauto& container_" + count_str + " = container_" + i_str + "[cfg." + key_name_str + "];\n";
				lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
			}
		}
		else if (key_str.find("lmb::RandomVec") != std::string::npos)
		{
			if (std::regex_search(key_name_str, sm, std::regex("rand_exclude")))
			{
				std::string insert_str = 
				"\tauto* container_" + count_str + "_ptr = container_" + i_str + ".RandomValueExclude(0 != " + key_name_str + ");\n" +
				"\tif (nullptr == container_" + count_str + "_ptr)\n" + 
				"\t{\n" + 
				"\t\treturn nullptr;\n" + 
				"\t}\n" +
				"\tauto& container_" + count_str + " = *container_" + count_str + "_ptr;\n";
				lmb::sed(m_gen_path.string(), 'O', "%%getfunc_content%%", insert_str);
			}
			else if (std::regex_search(key_name_str, sm, std::regex("rand")))
			{
				std::string insert_str = 
				"\tauto* container_" + count_str + "_ptr = container_" + i_str + ".RandomValue();\n" +
				"\tif (nullptr == container_" + count_str + "_ptr)\n" + 
				"\t{\n" + 
				"\t\treturn nullptr;\n" + 
				"\t}\n" +
				"\tauto& container_" + count_str + " = *container_" + count_str + "_ptr;\n";
				lmb::sed(m_gen_path.string(), 'O', "%%getfunc_content%%", insert_str);
			}

			{
				std::string insert_str;
				insert_str = insert_str + 
				"\t\t{\n" + 
				"\t\t\t" + this->CalcDynamicType(i + 1) + " tmp;\n" + 
				"\t\t\tcontainer_" + i_str + ".push_back(cfg." + key_name_str + ", tmp);\n" +
				"\t\t}\n" + 
				"\t\tauto& container_" + count_str + " = container_" + i_str + ".back();\n";
				lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
			}
		}
		{
			std::string insert_str; 
			if (i > 0)
			{
				insert_str = ", ";
			}
			insert_str = insert_str + "int " + key_name_str + "%%getfunc_args%%";
			lmb::sed(m_gen_path.string(), 's', "%%getfunc_args%%", insert_str);
		}
		if (i + 1 == m_key_vec.size())
		{
			{
				std::string insert_str = 
				"\treturn &container_" + count_str + ";";
				lmb::sed(m_gen_path.string(), 'O', "%%getfunc_content%%", insert_str);
			}

			{
				std::string insert_str = 
				"\t\tcontainer_" + count_str + " = cfg;\n\n" +
				"\t\tdataElement = dataElement.next_sibling();\n";
				lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
			}
		}
	}
	if (m_key_vec.empty())
	{
		{
			std::string insert_str = 
			"\treturn " + m_member_name + ";";
			lmb::sed(m_gen_path.string(), 'O', "%%getfunc_content%%", insert_str);
		}
		if (m_member_count > 0)
		{
			std::string insert_str = 
			"\t\ttmp_container = cfg;";
			lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
		}

		{
			std::string insert_str = 
			"\t\tbreak;";
			lmb::sed(m_gen_path.string(), 'O', "%%initfunc_content%%", insert_str);
		}
		lmb::sed(m_gen_path.string(), 's', "%%getfunc_pointer%%", "&");
	}
	else
	{
		lmb::sed(m_gen_path.string(), 's', "%%getfunc_pointer%%", "*");
	}


	{
		std::string insert_str = 
		"\t" + m_member_name+ " = std::move(tmp_container);";
		lmb::sed(m_gen_path.string(), 'O', "%%initfunc_end%%", insert_str);
	}

	if (m_key_vec.size() > 0)
	{
		std::string insert_str = 
		"const " + this->CalcDynamicType(0) + "& " + m_class_name + "::Get" + m_sub_class_name + "Container()\n" + 
		"{\n" + 
		"\treturn " + m_member_name + ";\n" + 
		"}\n";
		lmb::sed(m_gen_path.string(), 'O', "%%getfunc_container%%", insert_str);
	}

	lmb::sed(m_gen_path.string(), 'd', "%%initfunc_def_cfg%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%getfunc_content%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%initfunc_content%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%initfunc_end%%", "");
	lmb::sed(m_gen_path.string(), 's', "%%getfunc_args%%", "");
}

void GenCpp::Delete()
{
	GenBase::Delete();
	lmb::sed(m_gen_path.string(), 'd', "%%load_config%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%include itempool%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%include attribute%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%include droppool%%", "");
}
