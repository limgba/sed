#include "genhead.h"
#include "lmb_sed.h"

GenHead::GenHead(const std::filesystem::path& gen_path)
	: GenBase(gen_path)
{
}

void GenHead::Gen0(const std::string& struct_name)
{
	const std::string hump_struct_name = this->ToClassName(struct_name);
	const std::string class_name = m_class_name + hump_struct_name;
	{
		std::string insert_str = 
		"class " + class_name + "\n" + 
		"{\n" + 
		"\t" + class_name + "()\n" + 
		"\t\t:%%init_struct_head%%\n" + 
		"//%%init_struct_member%%\n" + 
		"\t{}\n" + 
		"//%%struct_member%%\n" + 
		"};";
		lmb::sed(m_gen_path.string(), 'O', "%%struct_name%%", insert_str);
		m_init_member_count = 0;
	}

	{
		std::string insert_str = 
		"\tint Init" + class_name + "Cfg(PugiXmlNode RootElement);";
		lmb::sed(m_gen_path.string(), 'O', "%%initfunc_name%%", insert_str);
	}

}
void GenHead::Gen1(const std::string& member_name)
{
	bool need_init = false;
	std::string member_type = this->CalcType(member_name, need_init);
	if (need_init)
	{
		lmb::sed(m_gen_path.string(), 's', "%%init_struct_comma%%", "");
		std::string insert_str = 
		"\t\t" + member_name + "(0),%%init_struct_comma%%";
		lmb::sed(m_gen_path.string(), 'O', "%%init_struct_member%%", insert_str);
		++m_init_member_count;
	}

	{
		std::string insert_str = 
		"\t" + member_type + " " + member_name + ";";
		lmb::sed(m_gen_path.string(), 'O', "%%struct_member%%", insert_str);
	}
}
void GenHead::Gen2()
{
	lmb::sed(m_gen_path.string(), 's', "%%init_struct_head%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%init_struct_member%%", "");
	lmb::sed(m_gen_path.string(), 's', ",%%init_struct_comma%%", "");
	lmb::sed(m_gen_path.string(), 'd', "%%struct_member%%", "");
}
