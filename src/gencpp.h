#ifndef __GENCPP_H__
#define __GENCPP_H__

#include "genbase.h"

class GenCpp : public GenBase
{
public:
	GenCpp(const std::filesystem::path& gen_path, const std::string& xml_name);
	virtual void Replace() override;
	virtual void Gen0(const std::string& struct_name) override;
	virtual void Gen1(const std::string& member_name) override;
	virtual void Gen2() override;
	virtual void Delete() override;
};

#endif
