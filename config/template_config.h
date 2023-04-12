#ifndef __%%def_name%%_H__
#define __%%def_name%%_H__

#include "servercommon/xml_adapter.hpp"

//%%struct_name%%

class %%class_name%% : virtual public ILogicConfig
{
public:
	%%class_name%%() = default;
	virtual ~%%class_name%%() = default;
	bool Init(const std::string& configname, std::string* err);

//%%getfunc_name%%

//%%getfunc_container%%
private:
	virtual int CheckData(const std::string& configname, std::string& check_data_func_name) = 0;
//%%initfunc_name%%

protected:
//%%member_name%%
};


#endif
