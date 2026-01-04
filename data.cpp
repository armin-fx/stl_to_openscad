#ifndef _data_cpp_
#define _data_cpp_

#include "data.h"

void data::load_arguments (int argc, char **argv)
{
	if (argc <= 1) throw display_help_short();
	
	bool do_show_help      = false;
	bool do_show_languages = false;
	
	for (int i=1; i<argc; ++i)
	{
		if (is_argument(argv[i]))
		{
			if      (equal_arguments(argv[i], "--help" ,"-?"))  do_show_help = true;
			else if (equal_arguments(argv[i], "--force","-f"))  do_overwrite = true;
			else if (equal_arguments(argv[i], "--language"))
			{
				if (i+1 < argc && ! is_argument(argv[i+1]))
				{
					text = get_language_object(argv[i+1]);
					++i;
				}
				else
					text = get_language_object("");
			}
			else if (equal_arguments(argv[i], "--language-list")) do_show_languages = true;
			else throw error_argument_unknown(argv[i]);
		}
		else
		{
			if      (input_filename .empty()) input_filename =argv[i];
			else if (output_filename.empty()) output_filename=argv[i];
		}
	}
	
	if (do_show_help)      throw display_help();
	if (do_show_languages) throw display_languages();
	
	if (input_filename.empty()) throw display_help_short();
	if (output_filename.empty())
	{
		if (input_filename != "-") output_filename = input_filename + ".scad";
		else                       output_filename = "FromSTL.scad";
	}
}

void data::prepare_and_identify_files ()
{
	if (input!=nullptr || output!=nullptr) close_files();
	//
	if (input_filename  == "-") input = &std::cin;
	else
	{
		input  = input_file  = new std::ifstream(input_filename, std::ifstream::binary);
		if (! input_file->is_open())
		{
			close_files();
			throw error_file_input_not_found();
		}
	}
	if (output_filename != "-")
	{
		if (! do_overwrite) if (file_exist(output_filename))
		{
			close_files();
			throw error_file_output_found();
		}
	}
	// lese die gesamte Datei in einen string
	//TODO hoher Speicherverbrauch, wird noch geändert
	input_as_string.clear();
	std::copy(
		std::istreambuf_iterator<char>(*input),
		std::istreambuf_iterator<char>(),
		std::back_inserter(input_as_string)
		);
	
	std::string first_bytes_with_header = input_as_string.substr(0, 86);
	if      (header_is_stl_ascii(first_bytes_with_header))
		input_type=file_type::stl_ascii;
	else if (header_is_stl_binary(first_bytes_with_header))
		input_type=file_type::stl_binary;
	else
	{
		input_type=file_type::unknown;
		input_as_string.clear();
		close_files();
		throw error_input_type_unknown();
	}
}

void data::open_output ()
{
	if (output_filename == "-") output = &std::cout;
	else
	{
		if (output_file != nullptr) delete output_file;
		output = output_file = new std::ofstream(output_filename, std::ofstream::binary);
		if (! output_file->is_open())
		{
			close_files();
			throw error_file_output_not_open();
		}
	}
}

void data::close_output ()
{
	if (output_file != nullptr) delete output_file;
	output = output_file = nullptr;
}

void data::close_input ()
{
	if (output_file != nullptr) delete output_file;
	output = output_file = nullptr;
}

void data::convert ()
{
	switch (input_type)
	{
		case file_type::stl_ascii:
			read_stl_ascii();
			break;
		case file_type::stl_binary:
			read_stl_binary();
			break;
		default:
			throw error_input_type_unknown();
			return;
	}
	
	write_scad();
}

void data::read_stl_ascii ()
{
	std::regex  pattern_model ("solid.*\\s?([\\s\\S]*?)endsolid");
	std::smatch matches_model;
	
	if (! std::regex_search(input_as_string, matches_model, pattern_model))
		throw error_input_empty();
	
	std::string model_data = matches_model[1];
	
	std::regex  pattern_vertex ("facet.*\\s?[\\s\\S]*?vertex(.*)[\\s\\S]*?vertex(.*)[\\s\\S]*?vertex(.*)[\\s\\S]*?endfacet");
	std::smatch matches_vertex;
	
	std::regex pattern_number ("\\s(.*)\\s(.*)\\s(.*)\\s(.*)\\s(.*)\\s(.*)\\s(.*)\\s(.*)\\s(.*)");
	
	while (std::regex_search(model_data, matches_vertex, pattern_vertex))
	{
		model_data = matches_vertex.suffix().str();
		
		std::string vertex_data = matches_vertex[1].str() + matches_vertex[2].str() + matches_vertex[3].str();
		std::smatch n;
		if (! std::regex_search(vertex_data, n, pattern_number))
			continue;
			
		stl.vertices.push_back( vertex_object_string(n[1],n[2],n[3]) );
		stl.vertices.push_back( vertex_object_string(n[4],n[5],n[6]) );
		stl.vertices.push_back( vertex_object_string(n[7],n[8],n[9]) );
	}
}

void data::read_stl_binary ()
{
	stl.vertices.clear();
	
	std::string::iterator p = input_as_string.begin();
	p += 80; // Header überspringen
	
	stl.triangle_count = read_value_little<uint32_t>(p, input_as_string.end());
	
	// REAL32[3] – Normal vector
	// REAL32[3] – Vertex 1
	// REAL32[3] – Vertex 2
	// REAL32[3] – Vertex 3
	// UINT16 – Attribute byte count

	float val[3];
	int n,i;
	while (p != input_as_string.end())
	{
		// Ignore Normal Vector
		for (i=0; i<3; ++i) read_value_little<float>(p, input_as_string.end());
		// All 3 Vertices
		for (n=0; n<3; ++n)
		{
			for (i=0; i<3; ++i)
			{
				val[i] = read_value_little<float>(p, input_as_string.end());
			}
			stl.vertices.push_back( vertex_object_string(val[0],val[1],val[2]) );
		}
		// Ignore attribute
		read_value_little<uint16_t>(p, input_as_string.end());
	}
}

void data::write_scad ()
{
	open_output();
	
	*output <<
		"object1();\n"
		"\n"
		"module object1() {\n"
		"p=[ "
		;
	int i=0;
	for (auto pos = stl.vertices.begin(); pos != stl.vertices.end(); ++pos, ++i)
	{
		if (pos != stl.vertices.begin()) *output << ',';
		if (i == 8)
		{
			*output << '\n';
			i = 0;
		}
		*output << '[' << (*pos)[0] << ',' << (*pos)[1] << ',' << (*pos)[2] << ']';
		
	}
	*output <<
		" ];\n"
		"f=[for (i=[0:3:len(p)-1]) [i,i+1,i+2]];\n"
		"polyhedron(points=p,faces=f);\n"
		"}\n"
		;
	
	close_output();
}

void data::write_scad_compressed ()
{
	open_output();
	
	*output <<
		"object1();\n"
		"\n"
		"module object1() {\n"
		"p=[ "
		;
	int i=0;
	for (auto pos = stl.vertices.begin(); pos != stl.vertices.end(); ++pos, ++i)
	{
		if (pos != stl.vertices.begin()) *output << ',';
		if (i == 8)
		{
			*output << '\n';
			i = 0;
		}
		*output << '[' << (*pos)[0] << ',' << (*pos)[1] << ',' << (*pos)[2] << ']';
		
	}
	*output <<
		" ];\n"
		"f=[for (i=[0:3:len(p)-1]) [i,i+1,i+2]];\n"
		"polyhedron(points=p,faces=f);\n"
		"}\n"
		;
	
	close_output();
}


#endif // _data_cpp_
