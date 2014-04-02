#include <GL/glew.h>
#include <soglu/FrameBufferObject.hpp>
#include <soglu/ErrorHandling.hpp>
#include "soglu/Attributes.hpp"
//#include "soglu/OGLTools.hpp"
#include "soglu/OGLDrawing.hpp"

namespace soglu {

Framebuffer::Framebuffer(): mInitialized( false ), mBinded( false )
{}

Framebuffer::~Framebuffer()
{
	finalize();
}

GLuint
Framebuffer::GetColorBuffer()
{
	SOGLU_ASSERT( mInitialized );
	return mColorTexture;
}

void
Framebuffer::initialize(int aWidth, int aHeight)
{
	initialize(aWidth, aHeight, GL_RGBA32F);
}

void
Framebuffer::initialize(int aWidth, int aHeight, GLint aInternalFormat)
{
	checkForGLError("Before framebuffer init.");
	SOGLU_ASSERT(isGLContextActive());
	mFrameBufferObject.initialize();
	mDepthBuffer.initialize();
	mColorTexture.initialize();
	mInitialized = true;
	resize( aWidth, aHeight, aInternalFormat );
}

void
Framebuffer::finalize()
{
	if ( mInitialized ) {
		SOGLU_ASSERT(isGLContextActive());
		mFrameBufferObject.finalize();
		mDepthBuffer.finalize();
		mColorTexture.finalize();
	}
	mInitialized = false;
}

void
Framebuffer::render()
{
	SOGLU_ASSERT(isGLContextActive());
	soglu::GLPushAtribs pushAttribs;
	GL_CHECKED_CALL(glDisable(GL_BLEND));
	GL_CHECKED_CALL( glMatrixMode( GL_PROJECTION ) );
	GL_CHECKED_CALL( glLoadIdentity() );
	GL_CHECKED_CALL( glMatrixMode( GL_MODELVIEW ) );
	GL_CHECKED_CALL( glLoadIdentity() );
	gl::activeTexture(TextureUnitId(0));
	GL_CHECKED_CALL( glBindTexture(GL_TEXTURE_2D, mColorTexture.value));
	GL_CHECKED_CALL( glDisable( GL_TEXTURE_1D ) );
	GL_CHECKED_CALL( glDisable( GL_TEXTURE_3D ) );
	GL_CHECKED_CALL( glEnable( GL_TEXTURE_2D ) );

	GL_CHECKED_CALL( gluOrtho2D( 0, (float)mSize[0], 0, (float)mSize[1] ) );

	drawTexturedQuad(
		glm::fvec2( 0.0f, 0.0f ),
		glm::fvec2( (float)mSize[0], (float)mSize[1] )
		);
	GL_CHECKED_CALL( glBindTexture( GL_TEXTURE_2D, 0 ) );
}

void
Framebuffer::bind()
{
	SOGLU_ASSERT(mInitialized);
	mFrameBufferObject.bind();
	mBinded = true;
}

void
Framebuffer::unbind()
{
	SOGLU_ASSERT(mInitialized);
	SOGLU_ASSERT(mBinded);
	mFrameBufferObject.unbind();
	mBinded = false;
}

void
Framebuffer::resize(int aWidth, int aHeight, GLint aInternalFormat)
{
	SOGLU_ASSERT(isGLContextActive());
	SOGLU_ASSERT ( mInitialized );
	SOGLU_DEBUG_PRINT("BEFORE BINDING FRAMEBUFFER OBJECT");
	auto framebufferBinder = getBinder(mFrameBufferObject);

	{
		auto renderbufferBinder = getBinder(mDepthBuffer);
		GL_CHECKED_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, aWidth, aHeight));
	}

	{
		auto textureBinder = getBinder(mColorTexture, GL_TEXTURE_2D);
		GL_CHECKED_CALL(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP ));
		GL_CHECKED_CALL(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP ));
		GL_CHECKED_CALL(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ));
		GL_CHECKED_CALL(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ));
		GL_CHECKED_CALL(glTexImage2D(
					GL_TEXTURE_2D,
					0,
					aInternalFormat,
					aWidth,
					aHeight,
					0,
					GL_RGBA,
					GL_FLOAT,
					nullptr
					));
	}

	GL_CHECKED_CALL( glFramebufferRenderbuffer(
				GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT,
				GL_RENDERBUFFER,
				mDepthBuffer.value
				) );

	GL_CHECKED_CALL( glFramebufferTexture2D(
				GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D,
				mColorTexture.value,
				0
				) );

	mSize.x = aWidth;
	mSize.y = aHeight;
}

void
Framebuffer::resize(int aWidth, int aHeight)
{
	resize(aWidth, aHeight, GL_RGBA32F);
}

} /*namespace soglu*/



