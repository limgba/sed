#include "genconfig.h"
#include "genbase.h"
#include "gencpp.h"
#include "genhead.h"
#include "lmb_sed.h"
#include "pugixml.hpp"
#include <set>
#include <memory>

void gen(const std::filesystem::path& xml_path, const std::filesystem::path& gen_path)
{
	if (!std::filesystem::exists(xml_path))
	{
		printf("gen xml[%s] not exists\n", xml_path.string().c_str());
		return;
	}
	if (!std::filesystem::exists(gen_path))
	{
		printf("gen xml[%s] not exists\n", gen_path.string().c_str());
		return;
	}
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xml_path.string().c_str());
	if (pugi::status_ok != result.status)
	{
		printf("read xml[%s] file fail\n", xml_path.string().c_str());
		return;
	}
	pugi::xml_node work_sheet_node = doc.document_element();
	if (work_sheet_node.empty())
	{
		printf("work_sheet_node is empty\n");
		return;
	}
	std::unique_ptr<GenBase> gen_base(nullptr);
	if (gen_path.extension().string() == ".h")
	{
		gen_base.reset(new GenHead(gen_path));
	}
	else if (gen_path.extension().string() == ".cpp")
	{
		gen_base.reset(new GenBase(gen_path));
	}
	else
	{
		printf("only gen c++ file[%s]\n", gen_path.string().c_str());
		return;
	}
	if (nullptr == gen_base)
	{
		printf("gen_base is nullptr\n");
		return;
	}
	gen_base->Replace();
	for (const auto& work_sheet : work_sheet_node)
	{
		const pugi::char_t* sheet_name = work_sheet.name();
		if (nullptr == sheet_name)
		{
			continue;
		}
		gen_base->Gen0(sheet_name);
		pugi::xml_node data_node = work_sheet_node.child(sheet_name);
		if (data_node.empty())
		{
			continue;
		}
		auto data = data_node.first_child();
		if (data.empty())
		{
			continue;
		}
		std::set<std::string> column_name_set;
		for (const auto& column : data)
		{
			if (column.empty())
			{
				break;
			}
			const pugi::char_t* column_name = column.name();
			if (nullptr == column_name)
			{
				break;
			}
			auto pair = column_name_set.insert(std::string(column_name));
			if (!pair.second)
			{
				continue;
			}
			gen_base->Gen1(column_name);
		}
		gen_base->Gen2();
	}
}
