#ifndef __%%def_name%%_CONFIG_H__
#define __%%def_name%%_CONFIG_H__

#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

//%%struct_name%%

namespace genconfig
{

class %%class_name%% : ConfigBase
{
public:
	bool Init(const std::string& configname, std::string* err);
//%%getfunc_name%%

private:
//%%initfunc_name%%

private:
//%%member_name%%

};


#endif
