#include "protocol_archiver.h"

base_archiver::cloudconfig_ptr base_archiver::_get_config_ptr()
{
	cloudconfig_ptr config = cloudconfig_;
	if (!tag_.empty()) config = _get_config_ptr(config, tag_);

	return config;
}

bool base_archiver::has_tag(const std::string& sub_tag)
{
	return _get_config_ptr(_get_config_ptr(), sub_tag) != nullptr;
}

bool base_archiver::has_tag(const std::string& sub_tag1, const std::string& sub_tag2)
{
	return _get_config_ptr(_get_config_ptr(_get_config_ptr(), sub_tag1), sub_tag2) != nullptr;
}

bool base_archiver::has_tag(const std::string& sub_tag1, const std::string& sub_tag2, const std::string& sub_tag3)
{
	return _get_config_ptr(_get_config_ptr(_get_config_ptr(_get_config_ptr(), sub_tag1), sub_tag2), sub_tag3) != nullptr;
}

base_archiver::cloudconfig_ptr base_archiver::_get_config_ptr(cloudconfig_ptr config, const std::string& tag)
{
	if (config == nullptr) return nullptr;

	if (config->subs.find(tag) == config->subs.end()) return nullptr;

	return config->subs[tag];
}

int base_archiver::_get_attr_value(cloudconfig_ptr config, const std::string& attr_name, int default_val)
{
	if (config == nullptr) return default_val;
	if (config->attrs_map.find(attr_name) == config->attrs_map.end()) return default_val;

	return atoi(config->attrs_map[attr_name].c_str());
}

const std::string& base_archiver::_get_attr_value(cloudconfig_ptr config, const std::string& attr_name, const std::string& default_val)
{
	if (config == nullptr) return default_val;
	if (config->attrs_map.find(attr_name) == config->attrs_map.end()) return default_val;

	return config->attrs_map[attr_name];
}

const std::string& base_archiver::get_value(const std::string& sub_tag, const std::string& attr_name, const std::string& default_val)
{
	return _get_attr_value(_get_config_ptr(_get_config_ptr(), sub_tag)
						, attr_name, default_val);
}

int base_archiver::get_value(const std::string& sub_tag, const std::string& attr_name, int default_val)
{
	return _get_attr_value(_get_config_ptr(_get_config_ptr(), sub_tag)
						, attr_name, default_val);
}

const std::string& base_archiver::get_value(const std::string& sub_tag1, const std::string& sub_tag2, const std::string& attr_name, const std::string& default_val)
{
	return _get_attr_value(_get_config_ptr(
							_get_config_ptr(
								_get_config_ptr(), 
								sub_tag1),
							sub_tag2), 
						attr_name, default_val);
}

int base_archiver::get_value(const std::string& sub_tag1, const std::string& sub_tag2, const std::string& attr_name, int default_val)
{
	return _get_attr_value(_get_config_ptr(
							_get_config_ptr(
								_get_config_ptr(), 
								sub_tag1),
							sub_tag2), 
						attr_name, default_val);
}

const std::string& base_archiver::get_value(const std::string& sub_tag1, const std::string& sub_tag2, const std::string& sub_tag3, 
											   const std::string& attr_name, const std::string& default_val)
{
	return _get_attr_value(_get_config_ptr(
							_get_config_ptr(
								_get_config_ptr(
									_get_config_ptr(), 
									sub_tag1),
								sub_tag2),
							sub_tag3
						),
						attr_name, default_val);
}

int base_archiver::get_value(const std::string& sub_tag1, const std::string& sub_tag2, const std::string& sub_tag3, const std::string& attr_name, int default_val)
{
	return _get_attr_value(_get_config_ptr(
							_get_config_ptr(
								_get_config_ptr(
									_get_config_ptr(), 
									sub_tag1),
								sub_tag2),
							sub_tag3
						),
						attr_name, default_val);
}

xml_archiver::xml_archiver(const std::string& main_tag, const std::string& tag)
	:	base_archiver(main_tag, tag)
{

}

xml_archiver::~xml_archiver()
{

}

bool xml_archiver::operator << (const std::string& data)
{
	CMarkup xml;
	if( !xml.SetDoc(data.c_str()))
	{
		return false;
	}

	cloudconfig_ = std::make_shared<cloudconfig_t>();
	cloudconfig_ptr config = cloudconfig_;
	xml.ResetPos();

	cloudconfig_->tag = main_tag_;
	if (!xml.FindElem(main_tag_.c_str())) return false;
	xml.IntoElem();

	if (!tag_.empty())
	{
		if (!xml.FindElem(tag_)) return false;
		config = std::make_shared<cloudconfig_t>();
		cloudconfig_->subs[tag_] = config;
		xml.IntoElem();
	}

	return _parse_elem(xml, config);
}

bool xml_archiver::_parse_elem(CMarkup& xml, cloudconfig_ptr config)
{
	while(xml.FindElem())
	{
		const std::string tag_name = xml.GetTagName();
		cloudconfig_ptr sub_config = std::make_shared<cloudconfig_t>();
		config->subs[tag_name] = sub_config;
		sub_config->tag = tag_name;
		int pos = 0;
		do
		{
			std::string attr_name = xml.GetAttribName(pos++);
			if (attr_name.empty()) break;
			sub_config->attrs_map[attr_name] = xml.GetAttrib(attr_name);
		} while (1);

		if (xml.IntoElem())
		{
			_parse_elem(xml, sub_config);
			xml.OutOfElem();
		}
	}

	return true;
}
