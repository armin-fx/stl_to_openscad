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
#include <regex>

#include "exception_own.h"
#include "file_type.h"
#include "data_helper.h"

extern text_basic* text;

template <typename floating>
class vertex_object_operation
{
public:
	floating vertex[3];
	
protected:
	void selfcopy (const vertex_object_operation& other)
	{
		for (int i=0; i<3; ++i)
			this->vertex[i] = other.vertex[i];
	}
public:
	inline floating& operator[] (std::size_t index)
	{	return vertex[index]; }
	
	inline bool operator< (const vertex_object_operation& a)
	{
		if (vertex[0]!=a.vertex[0]) {return vertex[0]<a.vertex[0];}
		if (vertex[1]!=a.vertex[1]) {return vertex[1]<a.vertex[1];}
		                             return vertex[2]<a.vertex[2];
	}
	
	inline bool operator== (const vertex_object_operation& a)
	{
		if (vertex[0]!=a.vertex[0]) {return false;}
		if (vertex[1]!=a.vertex[1]) {return false;}
		if (vertex[2]!=a.vertex[2]) {return false;}
		return true;
	}
};

class vertex_object_string : public vertex_object_operation<std::string>
{
public:	
	vertex_object_string (const vertex_object_string& other)
	{	selfcopy(other); }
	vertex_object_string& operator= (const vertex_object_string& other)
	{	selfcopy(other); return *this; }
	vertex_object_string (std::string& x, std::string& y, std::string& z)
	{
		this->vertex[0] = x;
		this->vertex[1] = y;
		this->vertex[2] = z;
	}
	template <typename floating>
	vertex_object_string (floating x, floating y, floating z)
	{
		this->vertex[0] = value_to_string(x);
		this->vertex[1] = value_to_string(y);
		this->vertex[2] = value_to_string(z);
	}
};

template <typename floating>
class vertex_object_value : public vertex_object_operation<floating>
{
public:	
	vertex_object_value (const vertex_object_value& other)
	{	selfcopy(other); }
	vertex_object_value& operator= (const vertex_object_value& other)
	{	selfcopy(other); return *this; }
	vertex_object_value (floating x, floating y, floating z)
	{
		this->vertex[0] = x;
		this->vertex[1] = y;
		this->vertex[2] = z;
	}
	vertex_object_value (std::string& x, std::string& y, std::string& z)
	{
		this->vertex[0] = string_to_floating<floating>(x);
		this->vertex[1] = string_to_floating<floating>(y);
		this->vertex[2] = string_to_floating<floating>(z);
	}
	
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
	bool do_write_faces = true;
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
