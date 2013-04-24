#pragma once

#include <string>
#include <GL/glew.h>
#include <boost/shared_array.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <cassert>

//#ifdef DEBUG_LEVEL
//#define GL_CHECKED_CALL( ... ) { __VA_ARGS__ ; checkForGLError( TO_STRING( __FILE__ " on " << __LINE__) ); }
//#else
#define GL_CHECKED_CALL( ... ) { __VA_ARGS__ ; }
//#endif /*DEBUG_LEVEL*/

#define SOGLU_ASSERT( ... ) { assert(__VA_ARGS__); }


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


//-------------------------------------------------------------------------------------------------------------------
//GLenum

#define TYPE_FROM_GL_b	int8_t
#define TYPE_FROM_GL_ub	uint8_t
#define TYPE_FROM_GL_s	int16_t
#define TYPE_FROM_GL_us	uint16_t
#define TYPE_FROM_GL_i	int32_t
#define TYPE_FROM_GL_ui	uint32_t
#define TYPE_FROM_GL_f	float
#define TYPE_FROM_GL_d	double

#define GL_FUNCTION_VECTOR_DEFINITION_MACRO( FUNC_NAME, GL_FUNC, TYPE_SUFFIX, DIM ) \
inline void FUNC_NAME( const glm::detail::tvec##DIM<TYPE_FROM_GL_##TYPE_SUFFIX> &coord )\
{ GL_FUNC##DIM## TYPE_SUFFIX ##v (glm::value_ptr(coord)); }

#define GL_VERTEX_VECTOR_DEFINITION_MACRO( TYPE_SUFFIX, DIM ) \
	GL_FUNCTION_VECTOR_DEFINITION_MACRO( GLVertexVector, glVertex, TYPE_SUFFIX, DIM );

#define GL_NORMAL_VECTOR_DEFINITION_MACRO( TYPE_SUFFIX, DIM ) \
	GL_FUNCTION_VECTOR_DEFINITION_MACRO( GLNormalVector, glNormal, TYPE_SUFFIX, DIM );

#define GL_COLOR_VECTOR_DEFINITION_MACRO( TYPE_SUFFIX, DIM ) \
	GL_FUNCTION_VECTOR_DEFINITION_MACRO( GLColorVector, glColor, TYPE_SUFFIX, DIM );

#define GL_TEXTURE_VECTOR_DEFINITION_MACRO( TYPE_SUFFIX, DIM ) \
	GL_FUNCTION_VECTOR_DEFINITION_MACRO( GLTextureVector, glTexCoord, TYPE_SUFFIX, DIM );

GL_VERTEX_VECTOR_DEFINITION_MACRO( s, 2 )
GL_VERTEX_VECTOR_DEFINITION_MACRO( i, 2 )
GL_VERTEX_VECTOR_DEFINITION_MACRO( f, 2 )
GL_VERTEX_VECTOR_DEFINITION_MACRO( d, 2 )

GL_VERTEX_VECTOR_DEFINITION_MACRO( s, 3 )
GL_VERTEX_VECTOR_DEFINITION_MACRO( i, 3 )
GL_VERTEX_VECTOR_DEFINITION_MACRO( f, 3 )
GL_VERTEX_VECTOR_DEFINITION_MACRO( d, 3 )

GL_VERTEX_VECTOR_DEFINITION_MACRO( s, 4 )
GL_VERTEX_VECTOR_DEFINITION_MACRO( i, 4 )
GL_VERTEX_VECTOR_DEFINITION_MACRO( f, 4 )
GL_VERTEX_VECTOR_DEFINITION_MACRO( d, 4 )

//-***************************************
GL_NORMAL_VECTOR_DEFINITION_MACRO( b, 3 )
GL_NORMAL_VECTOR_DEFINITION_MACRO( s, 3 )
GL_NORMAL_VECTOR_DEFINITION_MACRO( i, 3 )
GL_NORMAL_VECTOR_DEFINITION_MACRO( f, 3 )
GL_NORMAL_VECTOR_DEFINITION_MACRO( d, 3 )
//-***************************************
GL_COLOR_VECTOR_DEFINITION_MACRO(  b, 3 )
GL_COLOR_VECTOR_DEFINITION_MACRO( ub, 3 )
GL_COLOR_VECTOR_DEFINITION_MACRO(  s, 3 )
GL_COLOR_VECTOR_DEFINITION_MACRO( us, 3 )
GL_COLOR_VECTOR_DEFINITION_MACRO(  i, 3 )
GL_COLOR_VECTOR_DEFINITION_MACRO( ui, 3 )
GL_COLOR_VECTOR_DEFINITION_MACRO(  f, 3 )
GL_COLOR_VECTOR_DEFINITION_MACRO(  d, 3 )

GL_COLOR_VECTOR_DEFINITION_MACRO(  b, 4 )
GL_COLOR_VECTOR_DEFINITION_MACRO( ub, 4 )
GL_COLOR_VECTOR_DEFINITION_MACRO(  s, 4 )
GL_COLOR_VECTOR_DEFINITION_MACRO( us, 4 )
GL_COLOR_VECTOR_DEFINITION_MACRO(  i, 4 )
GL_COLOR_VECTOR_DEFINITION_MACRO( ui, 4 )
GL_COLOR_VECTOR_DEFINITION_MACRO(  f, 4 )
GL_COLOR_VECTOR_DEFINITION_MACRO(  d, 4 )
//-***************************************
/*GL_TEXTURE_VECTOR_DEFINITION_MACRO( s, 1 )
GL_TEXTURE_VECTOR_DEFINITION_MACRO( i, 1 )
GL_TEXTURE_VECTOR_DEFINITION_MACRO( f, 1 )
GL_TEXTURE_VECTOR_DEFINITION_MACRO( d, 1 )*/

GL_TEXTURE_VECTOR_DEFINITION_MACRO( s, 2 )
GL_TEXTURE_VECTOR_DEFINITION_MACRO( i, 2 )
GL_TEXTURE_VECTOR_DEFINITION_MACRO( f, 2 )
GL_TEXTURE_VECTOR_DEFINITION_MACRO( d, 2 )

GL_TEXTURE_VECTOR_DEFINITION_MACRO( s, 3 )
GL_TEXTURE_VECTOR_DEFINITION_MACRO( i, 3 )
GL_TEXTURE_VECTOR_DEFINITION_MACRO( f, 3 )
GL_TEXTURE_VECTOR_DEFINITION_MACRO( d, 3 )

GL_TEXTURE_VECTOR_DEFINITION_MACRO( s, 4 )
GL_TEXTURE_VECTOR_DEFINITION_MACRO( i, 4 )
GL_TEXTURE_VECTOR_DEFINITION_MACRO( f, 4 )
GL_TEXTURE_VECTOR_DEFINITION_MACRO( d, 4 )



} //namespace soglu
