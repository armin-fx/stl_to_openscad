#ifndef _text_en_h_
#define _text_en_h_
#include "text.h"

class text_EN_type : public text_basic
{
public:
	text_EN_type () : text_basic("en", "english") {}

	std::string help () const override { return std::string() +
		program_name + " - convert STL-files to OpenScad-files\n"
		"\n"
		"usage:  " + program_name + " [options] <infile> [outfile]\n"
		"\n"
		"    <infile> and/or <outfile> can be \"-\", which means stdin/stdout.\n"
		"\n"
		"    -?, --help               show help\n"
		"    -f, --force              force overwriting of output files\n"
		"        --language <token>   change the displayed language to the given\n"
		"                             locale-token\n"
		"        --language-list      lists all languages\n"
	;}
	std::string help_short () const override { return std::string() +
		program_name + " - convert STL-files to OpenScad-files\n"
		"\n"
		"usage:  " + program_name + " [options] <infile> [outfile]\n"
		"\n"
		"try this:\n"
		"    --help oder -?       show help\n"
	;}
	std::string error () const override { return std::string() +
		"An error has occurred.\n"
	;}
	std::string error_other (std::string text) const override { return std::string() +
		"An error has occurred: " + text + "\n"
	;}
	std::string unknown_argument (std::string argument) const override { return std::string() +
		"Unknown option: " + argument + "\n"
		"Show help with:\n"
		+ program_name + " --help\n"
	;}
	std::string file_input_not_found (std::string filename) const override { return std::string() +
		"Could not find input file: \"" + filename + "\"\n"
	;}
	std::string file_output_found (std::string filename) const override { return std::string() +
		"The output file already exists: \"" + filename + "\"\n"
		"Try option -f to overwrite output file.\n"
	;}
	std::string file_output_not_open (std::string filename) const override { return std::string() +
		"Can not open output file: \"" + filename + "\"\n"
	;}
	std::string input_type_unknown (std::string filename) const override { return std::string() +
		"Input file \"" + filename + "\" is no STL-file.\n"
	;}
	std::string input_type_undefined (std::string filename) const override { return std::string() +
		"Can not (yet) process the contents of the input file \"" + filename + "\".\n"
	;}
} text_EN;

#endif // _text_en_h_
