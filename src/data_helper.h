#ifndef _data_helper_h_
#define _data_helper_h_

#include <vector>
#include <cstring>
#include <string>
#include <algorithm>

#define SIZEOF_ARRAY(a) (sizeof (a) / sizeof *(a))
template <typename array_type> inline
std::size_t sizeof_array (const array_type a)
{
	return (sizeof a) / (sizeof *a);
}

template <typename object> inline
void test_show_object (object o, std::string name="")
{
	std::cerr << name << (name.empty() ? "" : " ")
		<< "-->" << (o) << "<--\n" << std::flush;
}

inline void print_normal (std::string str)
{
	std::cout << str << std::flush;
}
inline void print_error (std::string str)
{
	std::cerr << str << std::flush;
}

template <typename floating> inline
std::string value_to_string (floating value)
{
	std::ostringstream stream;
	stream << std::setprecision(std::numeric_limits<floating>::digits10 + 1) << value;
	return stream.str();
}

template <typename floating> inline
floating string_to_floating (std::string str)
{
	return 0;
}
template <> inline
float string_to_floating<float> (std::string str)
{
	return std::stof (str);
}
template <> inline
double string_to_floating<double> (std::string str)
{
	return std::stod (str);
}
template <> inline
long double string_to_floating<long double> (std::string str)
{
	return std::stold (str);
}

inline
float string_to_float (std::string str)
{
	return std::stof (str);
}
inline
double string_to_double (std::string str)
{
	return std::stod (str);
}
inline
long double string_to_long_double (std::string str)
{
	return std::stold (str);
}


template <typename type> inline
type read_value_little_unsigned (std::string::iterator& p, const std::string::iterator& end)
{
	type var = 0;
	for (int i=0; i<sizeof(type); ++i, ++p)
	{
		if (p==end) return 0;
		var |= (*p) << 8*i;
	}
	return var;
}
template <typename type> inline
type read_value_big_unsigned (std::string::iterator& p, const std::string::iterator& end)
{
	type var = 0;
	//
	for (int i=sizeof(type)-1; i>=0; --i, ++p)
	{
		if (p==end) return 0;
		var |= *p << 8*i;
	}
	return var;
}
template <typename type> inline
type read_value_little (std::string::iterator& p, const std::string::iterator& end)
{
	union
	{
		type value = 0;
		char  bytes[sizeof(type)];
	} field;
	for (int i=0; i<(int)sizeof(type); ++i, ++p)
	{
		if (p==end) return 0;
		field.bytes[i] = *p;
	}
	return field.value;
}
template <typename type> inline
type read_value_big (std::string::iterator& p, const std::string::iterator& end)
{
	union
	{
		type value = 0;
		char  bytes[sizeof(type)];
	} field;
	for (int i=(int)sizeof(type)-1; i>=0; --i, ++p)
	{
		if (p==end) return 0;
		field.bytes[i] = *p;
	}
	return field.value;
}


template <class ForwardIterator, class Container> inline
ForwardIterator search_all (ForwardIterator first, ForwardIterator last, Container container)
{	return search(first, last, std::begin(container), std::end(container)); }
template <class ForwardIterator, class Container, class BinaryPredicate> inline
ForwardIterator search_all (ForwardIterator first, ForwardIterator last, Container container, BinaryPredicate pred)
{	return search(first, last, std::begin(container), std::end(container), pred); }
//
template <class ForwardIterator> inline
ForwardIterator search_all
	(ForwardIterator first, ForwardIterator last, const char* str)
{	return search(first, last, str, str + std::strlen(str)); }
template <class ForwardIterator, class BinaryPredicate> inline
ForwardIterator search_all
	(ForwardIterator first, ForwardIterator last, const char* str, BinaryPredicate pred)
{	return search(first, last, str, str + std::strlen(str), pred); }

template <typename T> inline
void copy_array(T source, T destination)
{
	std::copy(std::begin(source), std::end(source), std::begin(destination));
}


inline bool equal_argument (const char* value, const char* argument)
{
	return strcmp(value, argument) == 0;
}
inline bool equal_arguments (const char* value, const char* a1, const char* a2=nullptr, const char* a3=nullptr)
{
	if                    (equal_argument(value, a1)) return true;
	if (a2 != nullptr) if (equal_argument(value, a2)) return true;
	if (a3 != nullptr) if (equal_argument(value, a3)) return true;
	return false;
}
inline bool is_argument (const char* value)
{
	return (strncmp(value, "-", 1) == 0) && (strlen(value) > 1);
}

inline bool file_exist (std::string filename)
{
	std::ifstream file (filename);
	if (file.is_open())
	{
		file.close();
		return true;
	}
	return false;
}

bool header_is_stl_ascii(std::string header)
{
	auto header_test = search_all(
		header.begin(), header.end(),
		"solid",
		[](char m, char n) { return std::tolower(m) == std::tolower(n); }
		);
	if (header_test == header.end()) return false;
	return true;
}
bool header_is_stl_binary(std::string header)
{
	if (header.size() < 82) return false;
	return true;
}

#endif // _data_helper_h_
