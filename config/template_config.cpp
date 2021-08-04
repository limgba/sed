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

	return true;
}

//%%getfunc_name%%

//%%getfunc_container%%


//%%initfunc_name%%
