#ifndef __LMB_SED_H__
#define __LMB_SED_H__

#include <string>

namespace lmb
{

bool sed(const std::string& file_name, char edit_type, const std::string& regex_str, const std::string& new_str);

}

#endif
