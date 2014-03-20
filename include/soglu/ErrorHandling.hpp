#pragma once

#include <cassert>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>

#include <exception>
#include <boost/exception/all.hpp>

#define SOGLU_D_PRINT(ARG)
#define SOGLU_LOG(ARG)

#define SOGLU_THROW(x) BOOST_THROW_EXCEPTION(x)

#define SOGLU_STRINGIFY(...) #__VA_ARGS__
#ifndef NDEBUG
#define GL_CHECKED_CALL( ... ) do { __VA_ARGS__ ; \
	soglu::checkForGLError( \
			std::string(__FILE__) \
			+ std::string(": ") \
			+ std::to_string(__LINE__) \
			+ std::string(": ") \
			+ SOGLU_STRINGIFY(__VA_ARGS__) ); } while(0);
#else
#define GL_CHECKED_CALL( ... ) do { __VA_ARGS__ ; } while (0);
#endif /*! NDEBUG*/

#define SOGLU_ASSERT( ... ) do { assert(__VA_ARGS__); } while (0);

#define GL_ERROR_CLEAR_AFTER_CALL( ... ) do { __VA_ARGS__ ; while (GL_NO_ERROR != glGetError()) {}; } while (0);

#define SOGLU_DEBUG_PRINT(...) \
	std::cout << __FILE__ \
		<< ":" << __LINE__ \
		<< ":" << __VA_ARGS__ << std::endl;

namespace soglu {

typedef boost::error_info<struct tag_message, std::string> MessageErrorInfo;

class GLException: public virtual boost::exception, public virtual std::exception
{
public:
	GLException() throw() {}
	GLException( std::string name ) throw() {}
	~GLException() throw(){}
};

void
checkForGLError(const std::string &situation);

bool
isGLContextActive();

} //namespace soglu
