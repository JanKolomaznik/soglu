#pragma once

#include <string>
#include <GL/glew.h>
#include <boost/shared_array.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <cassert>
#include <soglu/GLViewSetup.hpp>

//#ifdef DEBUG_LEVEL
//#define GL_CHECKED_CALL( ... ) { __VA_ARGS__ ; checkForGLError( TO_STRING( __FILE__ " on " << __LINE__) ); }
//#else
#define GL_CHECKED_CALL( ... ) { __VA_ARGS__ ; }
//#endif /*DEBUG_LEVEL*/

#define SOGLU_ASSERT( ... ) { assert(__VA_ARGS__); }

#define GL_ERROR_CLEAR_AFTER_CALL( ... ) { __VA_ARGS__ ; glGetError(); }

namespace soglu {

template< typename NType >
inline bool
intervalTest( NType a, NType b, NType val ) {
	if( val < a ) {
		return false;
	} 
	if( val > b ) {
		return false;
	} 

	return true;
}
	
glm::dvec3
getPointFromScreenCoordinates(glm::fvec2 aScreenCoords, const GLViewSetup &aViewSetup, double aZValue = 0.0);

template <typename TType>
glm::detail::tvec3<TType>
getDirectionFromScreenCoordinatesAndCameraPosition(const glm::detail::tvec2<TType> &aScreenCoords, const GLViewSetup &aViewSetup, const glm::detail::tvec3<TType> &aCameraPos )
{
	glm::detail::tvec3<TType> tmp(glm::unProject(
		glm::dvec3(aScreenCoords, 0.0),
		aViewSetup.modelView,
		aViewSetup.projection,
		aViewSetup.viewport
	));
	//std::cout << "aViewSetup.modelView = " << glm::to_string(aViewSetup.modelView) << std::endl;
	//std::cout << "aViewSetup.projection = " << glm::to_string(aViewSetup.projection) << std::endl;
	//std::cout << "aViewSetup.viewport = " << glm::to_string(aViewSetup.viewport) << std::endl;
	glm::detail::tvec3<TType> direction;
	/*GLint res = gluUnProject(
			aScreenCoords[0],  
			aScreenCoords[1],  
			0.0,  
			aViewSetup.model,  
			aViewSetup.proj,  
			aViewSetup.view,  
			&(objCoords1[0]),  
			&(objCoords1[1]),  
			&(objCoords1[2])
			);
	if( res == GLU_FALSE ) {
		_THROW_ GLException( "Cannot unproject screen coordinates" );
	}*/
		
	//LOG( "screen : " << aScreenCoords );
	//LOG( "coords1 : " << objCoords1 );
	direction = tmp - aCameraPos;
	//std::cout << "TMP = " << glm::to_string(tmp) << std::endl;
	//std::cout << "aCameraPos = " << glm::to_string(aCameraPos) << std::endl;
	//std::cout << "direction = " << glm::to_string(tmp) << std::endl;
	glm::normalize(direction);
	return direction;
}

inline void 
checkForGLError(const std::string &situation){}


/**
 * Basic space planes, each constant also defines index of axis perpendicular 
 * to given plane.
 **/
enum CartesianPlanes{
	YZ_PLANE = 0,
	XZ_PLANE = 1,
	XY_PLANE = 2
};
	
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
