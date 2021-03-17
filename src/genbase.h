#ifndef __GENBASE_H__
#define __GENBASE_H__

#include <string>
#include <vector>
#include <filesystem>

class GenBase
{
public:
	GenBase(const std::filesystem::path& gen_path);
	virtual void Replace();
	virtual void Gen0(const std::string& struct_name);
	virtual void Gen1(const std::string& member_name);
	virtual void Gen2();
	virtual void Delete();

protected:
	std::string ToDefName(const std::string& in_str);
	std::string ToFileName(const std::string& in_str);
	std::string ToClassName(const std::string& in_str);
	std::string CalcType(const std::string& in_str, bool& need_init);
protected:
	std::filesystem::path m_gen_path;
	std::string m_def_name;
	std::string m_file_name;
	std::string m_class_name;
	std::string m_sub_class_name;
	std::string m_member_name;
	int m_member_count;
	std::vector<std::string> m_key_vec;
	std::vector<std::string> m_key_name_vec;
};

#endif
