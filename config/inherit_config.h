#ifndef __%%def_name%%_CONFIG_H__
#define __%%def_name%%_CONFIG_H__

#include "%%file_name%%.h"


class %%class_name%% : virtual public %%base_class_name%%
{
public:
	%%class_name%%() = default;
	virtual ~%%class_name%%() = default;

	virtual bool OnInitFinish() override;
private:

};


#endif
