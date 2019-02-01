#ifndef ARCHIVER_CAIRUI
#define ARCHIVER_CAIRUI

#include <string>
#include <map>
#include <memory>
#include "Markup.h"

class base_archiver
{
public:
	base_archiver(const std::string& main_tag, const std::string& tag)
		:	main_tag_(main_tag),
			tag_(tag),
			cloudconfig_(nullptr)
	{}

	virtual ~base_archiver() {}

protected:
	typedef struct _cloudconfig_t
	{
		std::string tag;
		std::map<std::string, std::string> attrs_map;
		std::map<std::string, std::shared_ptr<_cloudconfig_t>> subs;
	}cloudconfig_t;

	typedef std::shared_ptr<cloudconfig_t> cloudconfig_ptr;

public:
	virtual bool operator << (const std::string& data) = 0;

	virtual bool has_tag(const std::string& sub_tag);
	virtual bool has_tag(const std::string& sub_tag1, const std::string& sub_tag2);
	virtual bool has_tag(const std::string& sub_tag1, const std::string& sub_tag2, const std::string& sub_tag3);

	virtual const std::string& get_value(const std::string& sub_tag, const std::string& attr_name, const std::string& default_val);
	virtual int get_value(const std::string& sub_tag, const std::string& attr_name, int default_val);

	virtual const std::string& get_value(const std::string& sub_tag1, const std::string& sub_tag2, const std::string& attr_name, const std::string& default_val);
	virtual int get_value(const std::string& sub_tag1, const std::string& sub_tag2, const std::string& attr_name, int default_val);

	virtual const std::string& get_value(const std::string& sub_tag1, const std::string& sub_tag2, const std::string& sub_tag3, 
										const std::string& attr_name, const std::string& default_val);
	virtual int get_value(const std::string& sub_tag1, const std::string& sub_tag2, const std::string& sub_tag3, const std::string& attr_name, int default_val);

private:
	cloudconfig_ptr _get_config_ptr();
	cloudconfig_ptr _get_config_ptr(cloudconfig_ptr config, const std::string& tag);
	int _get_attr_value(cloudconfig_ptr config, const std::string& attr_name, int default_val);
	const std::string& _get_attr_value(cloudconfig_ptr config, const std::string& attr_name, const std::string& default_val);

protected:
	std::shared_ptr<cloudconfig_t> cloudconfig_;
	std::string tag_;
	std::string main_tag_;
};


class xml_archiver : public base_archiver
{
public:
	xml_archiver(const std::string& main_tag, const std::string& tag = "");
	virtual ~xml_archiver();

public:
	virtual bool operator << (const std::string& data);

private:
	bool _parse_elem(CMarkup& xml, cloudconfig_ptr config);
};

#endif
