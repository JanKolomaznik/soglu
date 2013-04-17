#pragma once

#include <string>
#include <GL/glew.h>
#include <boost/shared_array.hpp>


//#ifdef DEBUG_LEVEL
//#define GL_CHECKED_CALL( ... ) { __VA_ARGS__ ; checkForGLError( TO_STRING( __FILE__ " on " << __LINE__) ); }
//#else
#define GL_CHECKED_CALL( ... ) { __VA_ARGS__ ; }
//#endif /*DEBUG_LEVEL*/

namespace soglu {

inline void 
checkForGLError(const std::string &situation){}
	
	
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

inline bool
isGLContextActive()
{
	return true;//return QGLContext::currentContext() != NULL; //TODO
}

void
initOpenGL();

void
getImageBufferFromTexture(size_t &aWidth, size_t &aHeight, boost::shared_array< uint8_t > &aBuffer, GLuint aTexture);

} //namespace soglu
