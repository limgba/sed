#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
%%include itempool%%
%%include attribute%%
%%include droppool%%
#include "%%file_name%%.h"

//%%cross_instance%%

bool %%class_name%%::Init(const std::string& configname, std::string* err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

//%%load_config%%

	int check_data_ret = this->CheckData();
	if (check_data_ret)
	{
		sprintf(errinfo, "%s: %%class_name%%::CheckData failed %d", configname.c_str(), check_data_ret);
		*err += errinfo;
		return false;
	}
	return true;
}

int %%class_name%%::CheckData()
{
	return 0;
}

//%%getfunc_name%%

//%%getfunc_container%%


//%%initfunc_name%%
