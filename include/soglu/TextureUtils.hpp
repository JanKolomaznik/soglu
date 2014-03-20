#pragma once

#if defined _WIN64 || defined _WIN32
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif //NOMINMAX
#	include <windows.h>
#	undef near
#	undef far
#endif

#include <GL/gl.h>
#include <boost/shared_array.hpp>

namespace soglu {

void
getImageBufferFromTexture(size_t &aWidth, size_t &aHeight, boost::shared_array< uint8_t > &aBuffer, GLuint aTexture);

} //namespace soglu
