%%include itempool%%
%%include itemconfigdata%%
%%include attribute%%
%%include droppool%%
#include "%%file_name%%.h"


bool %%class_name%%::Init(const std::string& configname, std::string* err)
{
	YY_XML_CONFIG_PRE_LOAD;

//%%load_config%%

	std::string check_data_func_name;
	int check_data_ret = this->CheckData(configname, check_data_func_name);
	if (check_data_ret)
	{
		sprintf(errinfo, "%s: %%class_name%%Impl::CheckData %s failed %d", configname.c_str(), check_data_func_name.c_str(), check_data_ret);
		*err += errinfo;
		return false;
	}
	return true;
}

//%%getfunc_name%%

//%%getfunc_container%%


//%%initfunc_name%%
