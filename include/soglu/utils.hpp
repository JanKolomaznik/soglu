#pragma once

#include <Cg/cg.h>    /* Can't include this?  Is Cg Toolkit installed! */
#include <Cg/cgGL.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <string>

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
