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
	initialize(int aWidth, int aHeight);

	void
	initialize(int aWidth, int aHeight, GLint aInternalFormat);

	void
	finalize();

	void
	render();

	void
	bind();

	void
	unbind();

	void
	resize(int aWidth, int aHeight, GLint aInternalFormat );

	void
	resize(int aWidth, int aHeight);

	glm::ivec2
	size()const
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
	glm::ivec2 mSize;
};

} /*namespace soglu*/


