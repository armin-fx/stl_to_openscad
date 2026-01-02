#ifndef _exception_own_
#define _exception_own_

#include <exception>

class exception_standard : public std::exception {};

class display_help           : public exception_standard {};
class display_help_short     : public exception_standard {};
class display_languages      : public exception_standard {};

class error_argument_unknown : public exception_standard
{
	char* text;
public:
	error_argument_unknown (char* _text = nullptr) noexcept { text = _text; }
	const char* what() const noexcept { return text; }
};
//class error_argument_input       : public exception_standard {};

class error_file_input_not_found : public exception_standard {};
class error_file_output_found    : public exception_standard {};
class error_file_output_not_open : public exception_standard {};
class error_input_type_unknown   : public exception_standard {};
class error_input_type_undefined : public exception_standard {};

#endif // _exception_own_
