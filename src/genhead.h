#ifndef __GENHEAD_H__
#define __GENHEAD_H__

#include "genbase.h"

class GenHead : public GenBase
{
public:
	GenHead(const std::filesystem::path& gen_path);
	virtual void Gen0(const std::string& struct_name) override;
	virtual void Gen1(const std::string& member_name) override;
	virtual void Gen2() override;
};

#endif
