#ifndef __GENHEAD_H__
#define __GENHEAD_H__

#include "genbase.h"

class GenHead : public GenBase
{
public:
	GenHead(const std::filesystem::path& gen_path, const std::string& xml_name);
	virtual void Replace() override;
	virtual void Gen0(const std::string& struct_name) override;
	virtual void Gen1(const std::string& member_name) override;
	virtual void Gen2() override;
	virtual void Delete() override;
private:
	int m_need_init_count;
};

#endif
