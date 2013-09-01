#pragma once

#include <cassert>
#include <string>

//#ifdef DEBUG_LEVEL
//#define GL_CHECKED_CALL( ... ) { __VA_ARGS__ ; checkForGLError( TO_STRING( __FILE__ " on " << __LINE__) ); }
//#else
#define GL_CHECKED_CALL( ... ) { __VA_ARGS__ ; }
//#endif /*DEBUG_LEVEL*/

#define SOGLU_ASSERT( ... ) { assert(__VA_ARGS__); }

#define GL_ERROR_CLEAR_AFTER_CALL( ... ) { __VA_ARGS__ ; glGetError(); }

namespace soglu {

inline void
checkForGLError(const std::string &situation){}


inline bool
isGLContextActive()
{
	return true;//return QGLContext::currentContext() != NULL; //TODO
}

} //namespace soglu
