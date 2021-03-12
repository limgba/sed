#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "obj/character/attribute.hpp"
#include "gameworld/checkresourcecenter.hpp"
#include "%%file_name%%.h"

bool %%class_name%%::Init(const std::string& configname, std::string* err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

//%%load_config%%

}

//%%getfunc_name%%


//%%initfunc_name%%
