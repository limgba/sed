#ifndef __GENBASE_H__
#define __GENBASE_H__

#include <string>
#include <vector>
#include <filesystem>
#include <unordered_set>
#include <unordered_map>

struct CalcTypeRet
{
	bool need_init = true;
	bool need_declare = true;
	std::string variable_name;
};

static const std::unordered_set<std::string> g_attribute_set = 
{
	"maxhp",
	"per_maxhp",
	"per_gongji",
	"movespeed",
	"shengming",
	"gongji",
	"fangyu",
	"pojia",
	"mingzhong",
	"shanbi",
	"baoji",
	"kangbao",
	"xianshu_gongji",
	"xianshu_fangyu",
	"move_speed_per",
	"shengming_qq",
	"fantan",
	"shanghai_zs",
	"fangyu_zs",
	"shengming_jc_per",
	"gongji_jc_per",
	"jianren_per",
	"chuantou_per",
	"mingzhong_jc_per",
	"shanghai_jc_per",
	"shanghai_jm_per",
	"zengshang_per",
	"jianshang_per",
	"zengshang_guaiwu_per",
	"jianshang_guaiwu_per",
	"jineng_shanghai_zj_per",
	"jineng_shanghai_jm_per",
	"shanbi_per",
	"mingzhong_per",
	"baoji_per",
	"kangbao_per",
	"baoji_shanghai_per",
	"baoji_shanghai_jm_per",
	"lianji_per",
	"lianji_dikang",
	"lianji_shanghai_per",
	"lianji_shanghai_jm_per",
	"jichuan_per",
	"jichuan_dikang",
	"gedang_per",
	"podang_per",
	"gedang_chuantou",
	"gedang_ms_per",
	"huixin_per",
	"huixin_zs",
	"huixin_js",
	"wanmei_per",
	"wanmei_dikang",
	"wanmei_zj",
	"wanmei_jm",
	"special_zj",
	"special_jm",
	"fantan_per",
	"fantan_dikang",
	"fantan_jc_per",
	"fantan_jm",
	"xianshu_gongji_per",
	"xianshu_fangyu_per",
	"shengming_hf_per",
	"zengshang_bs_per",
	"jianshang_bs_per",
	"control_zj",
	"control_jm",
	"qn_zs",
	"qn_js",
	"shanghai_jc_per_prof_1",
	"shanghai_jc_per_prof_2",
	"shanghai_jc_per_prof_3",
	"shanghai_jc_per_prof_4",
	"shanghai_jm_per_prof_1",
	"shanghai_jm_per_prof_2",
	"shanghai_jm_per_prof_3",
	"shanghai_jm_per_prof_4",
	"yushou_jingtong",
	"leifa_jingtong",
	"leifa_zengfu",
	"huofa_jingtong",
	"huofa_zengfu",
	"dufa_jingtong",
	"dufa_zengfu",
	"mufa_jingtong",
	"mufa_zengfu",
	"shanghai_quan_jc_per",
	"shanghai_quan_jm_per",
	"shengming_sxsh_per",
	"rare_exterior_rate_per",
	"rare_equip_rate_per",
	"shengming_zb_role_jc_per",
	"gongji_zb_role_jc_per",
	"fangyu_zb_role_jc_per",
	"pojia_zb_role_jc_per",
	"zengshang_boss_per",
	"jianshang_boss_per",
	"zhuoyue_per",
	"zhuoyue_dikang",
	"zhuoyue_zj",
	"zhuoyue_jm",
	"xingyun_per",
	"xingyun_dikang",
	"xingyun_zj",
	"xingyun_jm",


};

class GenBase
{
public:
	GenBase(const std::filesystem::path& gen_path);
	virtual void Replace();
	virtual void Gen0(const std::string& struct_name);
	virtual void Gen1(const std::string& member_name);
	virtual void Gen2();
	virtual void Delete();

protected:
	std::string ToDefName(const std::string& in_str);
	std::string ToFileName(const std::string& in_str);
	std::string ToClassName(const std::string& in_str);
	std::string CalcType(const std::string& in_str, CalcTypeRet& ret);
	std::string CalcDynamicType(size_t index);
	std::string MapToPair(const std::string& in_str);
protected:
	std::filesystem::path m_gen_path;
	std::string m_def_name;
	std::string m_file_name;
	std::string m_class_name;
	std::string m_base_class_name;
	std::string m_base_file_name;
	std::string m_sub_class_name;
	std::string m_member_name;
	int m_member_count;
	std::vector<std::string> m_key_vec;
	std::vector<std::string> m_key_name_vec;
	std::vector<std::string> m_column_name_vec;
	std::unordered_map<std::string, std::unordered_set<std::string>> m_declare_once_set_map;
};

#endif
