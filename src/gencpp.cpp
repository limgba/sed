#include "gencpp.h"
#include "lmb_sed.h"

GenCpp::GenCpp(const std::filesystem::path& gen_path)
	: GenBase(gen_path)
{
}

void GenCpp::Gen0(const std::string& struct_name)
{
	GenBase::Gen0(struct_name);

	{
		std::string insert_str = 
		"\tPUGI_XML_LOAD_CONFIG(\"" + struct_name + "\", Init" + m_sub_class_name + "Cfg);";
		lmb::sed(m_gen_path.string(), 'O', "%%load_config%%", insert_str);
	}

	{
		std::string insert_str = 
		"const " + m_sub_class_name + "* " + m_class_name + "::Get" + m_sub_class_name + "Cfg(%%getfunc_args%%\n{\n//%%getfunc_content%%\n}";
		lmb::sed(m_gen_path.string(), 'O', "%%getfunc_name%%", insert_str);
	}

	{
		std::string insert_str = 
		"int " + m_class_name + "::Init" + m_sub_class_name + "Cfg(PugiXmlNode RootElement)\n{\n\tPugiXmlNode dataElement = RootElement.child(\"data\");\n\tif (dataElement.empty())\n\t{\n\t\treturn -10000;\n\t}\n\n\twhile (!dataElement.empty())\n\t{\n\t\t" + m_sub_class_name + " cfg;\n//%%initfunc_content%%\n\t}\n}";
		lmb::sed(m_gen_path.string(), 'O', "%%initfunc_name%%", insert_str);
	}

}
void GenCpp::Gen1(const std::string& member_name)
{
	GenBase::Gen1(member_name);
}
void GenCpp::Gen2()
{
	GenBase::Gen2();
}
