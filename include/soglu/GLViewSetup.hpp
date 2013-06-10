#pragma once


#include <string>
#include <glm/glm.hpp>
//#include <glm/ext.hpp>

#include <soglu/Camera.hpp>
#include <glm/gtx/string_cast.hpp>

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

inline std::ostream &
operator<<(std::ostream &aStream, const GLViewSetup &aSetup)
{
	aStream << "Model matrix: \n" << glm::to_string(aSetup.model) << std::endl;
	aStream << "View matrix: \n" << glm::to_string(aSetup.view) << std::endl;
	aStream << "Projection matrix: \n" << glm::to_string(aSetup.projection) << std::endl;
	aStream << "Model-View matrix: \n" << glm::to_string(aSetup.modelView) << std::endl;
	aStream << "Model-View-Projection matrix: \n" << glm::to_string(aSetup.modelViewProj) << std::endl;
	aStream << "Viewport: " << glm::to_string(aSetup.viewport) << std::endl;
	return aStream;
}

void 
getCurrentGLSetup( soglu::GLViewSetup &aSetup );

GLViewSetup
getViewSetupFromCamera( const Camera &camera );

GLViewSetup
getViewSetupFromOrthoCamera( const OrthoCamera &camera );

} //namespace soglu
