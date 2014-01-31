#pragma once

#include <cassert>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>

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

#define GL_ERROR_CLEAR_AFTER_CALL( ... ) do { __VA_ARGS__ ; glGetError(); } while (0);

#define SOGLU_DEBUG_PRINT(...) \
	std::cout << __FILE__ \
		<< ":" << __LINE__ \
		<< ":" << __VA_ARGS__ << std::endl;

namespace soglu {

class GLException//: public std::exception
{
public:
	GLException() throw() {}
	GLException( std::string name ) throw() {}
	~GLException() throw(){}
};

void
checkForGLError(const std::string &situation);


inline bool
isGLContextActive()
{
	return true;//return QGLContext::currentContext() != NULL; //TODO
}

} //namespace soglu
