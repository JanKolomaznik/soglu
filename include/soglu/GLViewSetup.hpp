#pragma once


#include <string>
#include <glm/glm.hpp>
//#include <glm/ext.hpp>

#include "soglu/Camera.hpp"

namespace soglu {

class GLException
{
public:
	GLException( std::string name ) throw() {}
	~GLException() throw(){}
};

struct GLViewSetup
{
	GLViewSetup(): model(1.0), view(1.0), modelView(1.0), modelViewProj(1.0), projection(1.0)
	{}
	
	glm::dmat4x4 model;
	glm::dmat4x4 view;
	glm::dmat4x4 modelView;
	glm::dmat4x4 modelViewProj;
	glm::dmat4x4 projection;
	glm::ivec4  viewport;
};

void 
getCurrentGLSetup( soglu::GLViewSetup &aSetup );

GLViewSetup
getViewSetupFromCamera( const Camera &camera );

GLViewSetup
getViewSetupFromOrthoCamera( const OrthoCamera &camera );

} //namespace soglu
