#pragma once

inline void 
checkForGLError(const std::string &situation){}

//#ifdef DEBUG_LEVEL
//#define GL_CHECKED_CALL( ... ) { __VA_ARGS__ ; checkForGLError( TO_STRING( __FILE__ " on " << __LINE__) ); }
//#else
#define GL_CHECKED_CALL( ... ) { __VA_ARGS__ ; }
//#endif /*DEBUG_LEVEL*/

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

struct GLPushMatrices
{
	GLPushMatrices()
	{
		GL_CHECKED_CALL( glMatrixMode(GL_PROJECTION) );
		GL_CHECKED_CALL( glPushMatrix() );
		GL_CHECKED_CALL( glMatrixMode(GL_MODELVIEW) );
		GL_CHECKED_CALL( glPushMatrix() );
	}
	~GLPushMatrices()
	{
		GL_CHECKED_CALL( glMatrixMode(GL_MODELVIEW) );
		GL_CHECKED_CALL( glPopMatrix() );
		GL_CHECKED_CALL( glMatrixMode(GL_PROJECTION) );
		GL_CHECKED_CALL( glPopMatrix() );
	}
};

} //namespace soglu
