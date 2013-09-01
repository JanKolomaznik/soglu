#pragma once

#include <GL/gl.h>
#include <soglu/ErrorHandling.hpp>

namespace soglu {

struct GLPushAtribs
{
	GLPushAtribs(GLbitfield attribs = GL_ALL_ATTRIB_BITS )
	{
		GL_CHECKED_CALL( glPushAttrib( attribs ) );
	}
	~GLPushAtribs()
	{
		GL_CHECKED_CALL( glPopAttrib() );
	}
};


} //namespace soglu

