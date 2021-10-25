#ifndef __%%def_name%%_H__
#define __%%def_name%%_H__

#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

//%%struct_name%%

class %%class_name%% : virtual public ConfigBase
{
public:
	%%class_name%%() = default;
	virtual ~%%class_name%%() = default;
	//%%cross_instance%%
	bool Init(const std::string& configname, std::string* err);
private:
	virtual int CheckData();
//%%getfunc_name%%

//%%getfunc_container%%
private:
//%%initfunc_name%%

protected:
//%%member_name%%
};


#endif
