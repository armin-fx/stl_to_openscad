// stl_to_openscad - Wandelt STL-Dateien in OpenScad-Dateien um
//

#include <exception>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>

#include "text.h"
#include "text_en.h"
#include "text_de.h"
#include "data.h"

#include "data.cpp"

data        d;
text_basic* text = get_language_object();

int main(int argc, char **argv)
{
	try
	{
		//std::cerr << "-->" << std::setprecision(std::numeric_limits<float>::digits10 + 1)  << value_to_float(std::string("3.141592654e+900")) << "<--\n" << std::flush;
		d.load_arguments (argc, argv);
		d.prepare_and_identify_files();
		//test_show_object(d.input_type);
		d.convert();
	}
	catch(display_help&)
	{
		std::cout << text->help() << std::flush;
	}
	catch(display_help_short&)
	{
		std::cout << text->help_short() << std::flush;
	}
	catch(display_languages&)
	{
		std::cout << list_all_languages() << std::flush;
	}
	catch(error_argument_unknown& e)
	{
		std::cerr << text->unknown_argument(e.what()) << std::flush;
		return EXIT_FAILURE;
	}
	catch(error_file_input_not_found&)
	{
		std::cerr << text->file_input_not_found(d.input_filename) << std::flush;
		return EXIT_FAILURE;
	}
	catch(error_file_output_found&)
	{
		std::cerr << text->file_output_found(d.output_filename) << std::flush;
		return EXIT_FAILURE;
	}
	catch(error_file_output_not_open&)
	{
		std::cerr << text->file_output_not_open(d.output_filename) << std::flush;
		return EXIT_FAILURE;
	}
	catch(error_input_type_unknown&)
	{
		std::cerr << text->input_type_unknown(d.input_filename) << std::flush;
		return EXIT_FAILURE;
	}
	catch(error_input_type_undefined&)
	{
		std::cerr << text->input_type_undefined(d.input_filename) << std::flush;
		return EXIT_FAILURE;
	}
	catch(error_input_empty&)
	{
		std::cerr << text->input_empty(d.input_filename) << std::flush;
		return EXIT_FAILURE;
	}
	catch(std::exception& e)
	{
		std::cerr << text->error_other(e.what()) << std::flush;
		return EXIT_FAILURE;
	}
	catch(...)
	{
		std::cerr << text->error() << std::flush;
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

