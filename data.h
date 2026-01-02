#ifndef _data_h_
#define _data_h_

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>

#include "exception_own.h"
#include "file_type.h"
#include "data_helper.h"

extern text_basic* text;


class vertex_object_string
{
public:
	std::string vertex[3];
private:
	void selfcopy (const vertex_object_string& other)
	{
		for (int i=0; i<3; ++i)
			vertex[i] = other.vertex[i];
	}
public:	
	vertex_object_string (const vertex_object_string& other)
	{	selfcopy(other); }
	vertex_object_string& operator= (const vertex_object_string& other)
	{	selfcopy(other); return *this; }
	vertex_object_string (std::string x, std::string y, std::string z)
	{
		vertex[0] = x;
		vertex[1] = y;
		vertex[2] = z;
	}
	template <typename floating>
	vertex_object_string (floating x, floating y, floating z)
	{
		vertex[0] = value_to_string(x);
		vertex[1] = value_to_string(y);
		vertex[2] = value_to_string(z);
	}
	
	inline std::string& operator[] (std::size_t index)
	{	return vertex[index]; }
};

template <typename floating>
class vertex_object_value
{
public:
	floating vertex[3];
private:
	void selfcopy (const vertex_object_value& other)
	{
		for (int i=0; i<3; ++i)
			vertex[i] = other.vertex[i];
	}
public:	
	vertex_object_value (const vertex_object_value& other)
	{	selfcopy(other); }
	vertex_object_value& operator= (const vertex_object_value& other)
	{	selfcopy(other); return *this; }
	vertex_object_value (floating x, floating y, floating z)
	{
		vertex[0] = x;
		vertex[1] = y;
		vertex[2] = z;
	}
	vertex_object_value (std::string x, std::string y, std::string z)
	{
		vertex[0] = string_to_floating<floating>(x);
		vertex[1] = string_to_floating<floating>(y);
		vertex[2] = string_to_floating<floating>(z);
	}
	
	inline floating& operator[] (std::size_t index)
	{	return vertex[index]; }
};

struct stl_object
{
	unsigned long                     triangle_count = 0;
	std::vector<vertex_object_string> vertices;
};

class data
{
protected:
	std::ifstream* input_file  = nullptr;
	std::ofstream* output_file = nullptr;
	//
	std::string    input_as_string;
public:
	file_type      input_type = file_type::unknown;
	std::string    input_filename;
	std::string    output_filename;
	std::istream*  input  = nullptr;
	std::ostream*  output = nullptr;
	bool do_overwrite = false;
	//
	bool do_find_double_vertices = false;
	bool do_write_faces          = false;
	//
	stl_object stl;
	
	~data ()
	{
		close_files();
	}
	
	void load_arguments (int argc, char **argv);
	void prepare_and_identify_files ();
	void open_output ();
	void close_output ();
	void close_input ();
	inline
	void close_files ();
	void read_stl_ascii ();
	void read_stl_binary ();
	void convert ();
	void write_scad ();
	void write_scad_compressed ();
};


void data::close_files()
{
	close_output();
	close_input();
}

#endif // _data_h_
