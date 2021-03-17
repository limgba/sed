#include "genhead.h"
#include "lmb_sed.h"

GenHead::GenHead(const std::filesystem::path& gen_path)
	: GenBase(gen_path)
{
}

void GenHead::Gen0(const std::string& struct_name)
{
	GenBase::Gen0(struct_name);
	{
		std::string insert_str = 
		"class " + m_sub_class_name + "\n" + 
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
	bool need_init = false;
	std::string member_type = this->CalcType(member_name, need_init);
	if (need_init)
	{
		lmb::sed(m_gen_path.string(), 's', "%%init_struct_comma%%", "");
		std::string insert_str = 
		"\t\t" + member_name + "(0),%%init_struct_comma%%";
		lmb::sed(m_gen_path.string(), 'O', "%%init_struct_member%%", insert_str);
	}

	{
		std::string insert_str = 
		"\t" + member_type + " " + member_name + ";";
		lmb::sed(m_gen_path.string(), 'O', "%%struct_member%%", insert_str);
	}
}
void GenHead::Gen2()
{
	GenBase::Gen2();
	lmb::sed(m_gen_path.string(), 's', "%%init_struct_head%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%init_struct_member%%", "");
	lmb::sed(m_gen_path.string(), 's', ",%%init_struct_comma%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%struct_member%%", "");

	{
		std::string member_name = "\t";
		for (const auto& key : m_key_vec)
		{
			member_name += key;
		}
		member_name = member_name + m_sub_class_name + std::string(m_key_vec.size(), '>') + m_member_name + ";";
		lmb::sed(m_gen_path.string(), 'O', "%%member_name%%", member_name);
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
	lmb::sed(m_gen_path.string(), 'd', "%%struct_name%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%member_name%%", "");

}
