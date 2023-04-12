#ifndef __%%def_name%%_H__
#define __%%def_name%%_H__

#include "%%base_file_name%%.h"


class %%class_name%% : public %%base_class_name%%
{
public:
	%%class_name%%() = default;
	virtual ~%%class_name%%() = default;
	static const %%class_name%%* Get();

private:
	virtual int CheckData(const std::string& configname, std::string& check_data_func_name) override;

};


#endif
