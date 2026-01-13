#ifndef _window_stream_h_
#define _window_stream_h_

template <typename char_type, typename traits = std::char_traits<char_type> >
class basic_windowstream : public std::basic_streambuf<char_type, traits>
{
private:
	char_type* buffer;
	
public:
	typedef typename traits::int_type int_type;
	
	basic_teebuf (std::basic_streambuf<char_type, traits>* _stream1)
		: stream1(_stream1)
	{}
	
}


#endif // _window_stream_h_
