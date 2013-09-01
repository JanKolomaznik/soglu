#pragma once

#include <GL/gl.h>
#include <boost/shared_array.hpp>

namespace soglu {

void
getImageBufferFromTexture(size_t &aWidth, size_t &aHeight, boost::shared_array< uint8_t > &aBuffer, GLuint aTexture);

} //namespace soglu
