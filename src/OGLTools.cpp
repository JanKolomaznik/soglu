#include <GL/glew.h>
#include "soglu/OGLTools.hpp"
#include "soglu/OGLDrawing.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <boost/format.hpp>


namespace soglu
{

glm::dvec3
getPointFromScreenCoordinates(glm::fvec2 aScreenCoords, const GLViewSetup &aViewSetup, double aZValue)
{
	glm::dvec3 tmp = glm::unProject(
		glm::dvec3( aScreenCoords[0], aScreenCoords[1], aZValue),
		aViewSetup.modelView,
		aViewSetup.projection,
		aViewSetup.viewport
	);
	return tmp;
}


#ifdef DISABLE_0
void
getCurrentGLSetup(soglu::GLViewSetup &aSetup)
{
	glGetDoublev( GL_PROJECTION_MATRIX, glm::value_ptr( aSetup.projection) );
	glGetDoublev( GL_MODELVIEW_MATRIX, glm::value_ptr( aSetup.modelView ) );
	aSetup.view = aSetup.modelView;
	aSetup.model = glm::dmat4x4( 1.0 );
	aSetup.modelViewProj = aSetup.projection * aSetup.modelView;
	glGetIntegerv( GL_VIEWPORT, glm::value_ptr( aSetup.viewport ) );
	CheckForGLError( "getCurrentGLSetup()" );
};


std::ostream &
operator<<( std::ostream & stream, const GLViewSetup &setup )
{
	stream << "model:\n" << setup.model;
	stream << "view:\n" << setup.view;
	stream << "projection:\n" << setup.projection;
	stream << "modelView:\n" << setup.modelView;
	stream << "modelViewProj:\n" << setup.modelViewProj;
	return stream;
}






void
checkForGLError( const std::string &situation  )
{
	GLenum errorCode = glGetError();
	if (errorCode != GL_NO_ERROR) {
		const char *string = (const char *)gluErrorString(errorCode);
		throw GLException( TO_STRING( situation << " : " << string ) );
	}
}


void
getImageBufferFromTexture( uint32 &aWidth, uint32 &aHeight, boost::shared_array< uint8 > &aBuffer, GLuint aTexture )
{
	GLint width = 0;
	GLint height = 0;
	GL_CHECKED_CALL( glBindTexture( GL_TEXTURE_2D, aTexture ) );
	GL_CHECKED_CALL( glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width ) );
	GL_CHECKED_CALL( glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height ) );

	aBuffer = boost::shared_array< uint8 >( new uint8[ 3 * width * height ] );

	GL_CHECKED_CALL( glGetTexImage(
				GL_TEXTURE_2D,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				(void*)aBuffer.get()
				) );
	GL_CHECKED_CALL( glBindTexture( GL_TEXTURE_2D, 0 ) );
}

#endif //DISABLE_0

void
initOpenGL()
{
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		throw "EInitError";//( "GLEW" );
	}

	std::cout << boost::format("Status: Using GLEW %1%\n") % glewGetString(GLEW_VERSION);
	/*LOG( "Status: Using GLEW " << glewGetString(GLEW_VERSION) );
	LOG( "\tGLEW_VERSION_1_1 " << ((GLEW_VERSION_1_1) ? std::string("OK") : std::string("FAIL")) );
	LOG( "\tGLEW_VERSION_1_2 " << ((GLEW_VERSION_1_2) ? std::string("OK") : std::string("FAIL")) );
	LOG( "\tGLEW_VERSION_1_3 " << ((GLEW_VERSION_1_3) ? std::string("OK") : std::string("FAIL")) );
	LOG( "\tGLEW_VERSION_1_4 " << ((GLEW_VERSION_1_4) ? std::string("OK") : std::string("FAIL")) );
	LOG( "\tGLEW_VERSION_1_5 " << ((GLEW_VERSION_1_5) ? std::string("OK") : std::string("FAIL")) );
	LOG( "\tGLEW_VERSION_2_0 " << ((GLEW_VERSION_2_0) ? std::string("OK") : std::string("FAIL")) );
	LOG( "\tGLEW_VERSION_2_1 " << ((GLEW_VERSION_2_1) ? std::string("OK") : std::string("FAIL")) );
	LOG( "\tGLEW_VERSION_3_0 " << ((GLEW_VERSION_3_0) ? std::string("OK") : std::string("FAIL")) );
	LOG( "\tGLEW_VERSION_3_1 " << ((GLEW_VERSION_3_1) ? std::string("OK") : std::string("FAIL")) );
	LOG( "\tGLEW_VERSION_3_2 " << ((GLEW_VERSION_3_2) ? std::string("OK") : std::string("FAIL")) );*/
}

void
getImageBufferFromTexture(size_t &aWidth, size_t &aHeight, boost::shared_array< uint8_t > &aBuffer, GLuint aTexture)
{
	GLint width = 0;
	GLint height = 0;
	GL_CHECKED_CALL( glBindTexture( GL_TEXTURE_2D, aTexture ) );
	GL_CHECKED_CALL( glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width ) );
	GL_CHECKED_CALL( glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height ) );

	aBuffer = boost::shared_array< uint8_t >( new uint8_t[ 3 * width * height ] );

	GL_CHECKED_CALL( glGetTexImage(
				GL_TEXTURE_2D,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				(void*)aBuffer.get()
				) );
	GL_CHECKED_CALL( glBindTexture( GL_TEXTURE_2D, 0 ) );
}

} /*namespace soglu*/

