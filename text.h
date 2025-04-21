#ifndef _text_h_
#define _text_h_

#include <map>
#include <string>
#include <cstdlib>

const char* program_name    = "STL_to_OpenScad";
const char* program_version = "alpha";

class text_list
{
public:
	virtual std::string help () const = 0;
	virtual std::string help_short () const = 0;
	virtual std::string error () const = 0;
	virtual std::string error_other (std::string text) const = 0;
	virtual std::string unknown_argument (std::string argument) const = 0;
	virtual std::string file_input_not_found (std::string filename) const = 0;
	virtual std::string file_output_found (std::string filename) const = 0;
	virtual std::string file_output_not_open (std::string filename) const = 0;
	virtual std::string input_type_unknown (std::string filename) const = 0;
};

class text_basic_type : public text_list
{
public:
	virtual const std::string& get_locale()   const = 0;
	virtual const std::string& get_language() const = 0;
};

typedef std::map<std::string, text_basic_type*> languages_type;
languages_type languages;

class text_basic : public text_basic_type
{
private:
	const std::string locale, language;
public:
	text_basic (const std::string lang_token, const std::string lang_name="") :
		locale  (lang_token),
		language(lang_name)
	{
		languages.insert(languages_type::value_type(locale, this));
	}
	
	const std::string& get_locale()   const override { return locale; }
	const std::string& get_language() const override { return language; }
};

std::string get_language_string ()
{
	// work in linux and wine
	std::string lang = std::getenv("LANG");
	return lang;
}

text_basic* get_language_object (std::string locale)
{
	if (locale.size() >= 2)
	{
		if (locale.size() > 5)
		{
			// sucht nach vollständigen lokalen Sprachkürzel (z.B de_DE.UTF-8)
			auto entry = languages.find(locale);
			if (entry != languages.end()) return (text_basic*)entry->second;
		}
		if (locale.size() > 2)
		{
			// sucht nach lokalen Sprachkürzel (z.B de_DE, de_AT)
			auto entry = languages.find(locale.substr(0,5));
			if (entry != languages.end()) return (text_basic*)entry->second;
		}
		// sucht nach kurzen lokalen Sprachkürzel (z.B de, en)
		auto entry = languages.find(locale.substr(0,2));
		if (entry != languages.end()) return (text_basic*)entry->second;
	}
	// Rückfallabsicherung auf englisch
	auto entry = languages.find("en");
	if (entry != languages.end()) return (text_basic*)entry->second;

	std::cerr << "No language found!" << std::endl;
	return nullptr;
}
inline text_basic* get_language_object () { return get_language_object(get_language_string()); }

std::string list_all_languages ()
{
	std::string str;
	for (languages_type::iterator entry=languages.begin(); entry!=languages.end(); ++entry)
	{
		str += entry->first;
		str.append(std::max<int>(1, 12 - entry->first.size()), ' ');
		str += "=> " + entry->second->get_language() + '\n';
	}
	return str;
}

#endif // _text_h_
