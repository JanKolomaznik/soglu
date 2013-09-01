#pragma once

#include <string>
//#include <GL/glew.h>
#include <boost/shared_array.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <soglu/GLViewSetup.hpp>
#include <soglu/ErrorHandling.hpp>
#include <soglu/Attributes.hpp>
#include <soglu/Init.hpp>
#include <soglu/GLWrappers.hpp>

namespace soglu {

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
