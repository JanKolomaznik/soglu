#pragma once

/*#include "MedV4D/Common/Common.h"
#include "MedV4D/GUI/utils/OGLDrawing.h"
#include "MedV4D/GUI/utils/OGLTools.h"*/

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/ext.hpp>

namespace soglu {

class FrameBufferObject
{
public:
	FrameBufferObject();

	~FrameBufferObject();

	GLuint
	GetColorBuffer();

	void
	Initialize( unsigned aWidth, unsigned aHeight );

	void
	Initialize( unsigned aWidth, unsigned aHeight, GLint aInternalFormat );

	void
	Finalize();

	void
	Render();

	void
	Bind();

	void
	Unbind();

	void
	Resize( unsigned aWidth, unsigned aHeight, GLint aInternalFormat = GL_RGBA32F );

	glm::uvec2
	GetSize()const
	{ return mSize; }

	bool
	isInitialized() const
	{ return mInitialized; }
protected:
	GLuint	mFrameBufferObject,
		mDepthBuffer,
		mColorTexture;
	bool mInitialized;
	bool mBinded;
	glm::uvec2 mSize;
};

} /*namespace soglu*/


