#include <GL/glew.h>
#include <soglu/FrameBufferObject.hpp>
#include <soglu/ErrorHandling.hpp>
#include "soglu/Attributes.hpp"
//#include "soglu/OGLTools.hpp"
#include "soglu/OGLDrawing.hpp"

namespace soglu {

FrameBufferObject::FrameBufferObject(): mInitialized( false ), mBinded( false )
{}

FrameBufferObject::~FrameBufferObject()
{
	Finalize();
}

GLuint
FrameBufferObject::GetColorBuffer()
{
	SOGLU_ASSERT( mInitialized );
	return mColorTexture;
}

void
FrameBufferObject::Initialize( unsigned aWidth, unsigned aHeight )
{
	Initialize( aWidth, aHeight, GL_RGBA32F );
}

void
FrameBufferObject::Initialize( unsigned aWidth, unsigned aHeight, GLint aInternalFormat )
{
	checkForGLError("Before framebuffer init.");
	SOGLU_ASSERT(isGLContextActive());
	GL_CHECKED_CALL( glGenFramebuffers( 1, &mFrameBufferObject ) );
	GL_CHECKED_CALL( glGenRenderbuffers( 1, &mDepthBuffer ) );
	GL_CHECKED_CALL( glGenTextures( 1, &mColorTexture ) );

	mInitialized = true;
	Resize( aWidth, aHeight, aInternalFormat );
}

void
FrameBufferObject::Finalize()
{
	if ( mInitialized ) {
		SOGLU_ASSERT(isGLContextActive());
		GL_CHECKED_CALL( glDeleteFramebuffers( 1, &mFrameBufferObject ) );
		GL_CHECKED_CALL( glDeleteTextures( 1, &mColorTexture ) );
		GL_CHECKED_CALL( glDeleteRenderbuffers( 1, &mDepthBuffer ) );
	}
	mInitialized = false;
}

void
FrameBufferObject::Render()
{
	SOGLU_ASSERT(isGLContextActive());
	soglu::GLPushAtribs pushAttribs;
	GL_CHECKED_CALL( glMatrixMode( GL_PROJECTION ) );
	GL_CHECKED_CALL( glLoadIdentity() );
	GL_CHECKED_CALL( glMatrixMode( GL_MODELVIEW ) );
	GL_CHECKED_CALL( glLoadIdentity() );
	GL_CHECKED_CALL( glBindTexture( GL_TEXTURE_2D, mColorTexture ) );
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
FrameBufferObject::Bind()
{
	SOGLU_ASSERT( mInitialized );
	GL_CHECKED_CALL( glBindFramebuffer( GL_FRAMEBUFFER, mFrameBufferObject ) );
	mBinded = true;
}

void
FrameBufferObject::Unbind()
{
	SOGLU_ASSERT( mInitialized );
	GL_CHECKED_CALL( glBindFramebuffer( GL_FRAMEBUFFER, 0 ) );
	mBinded = false;
}

void
FrameBufferObject::Resize( unsigned aWidth, unsigned aHeight, GLint aInternalFormat)
{
	SOGLU_ASSERT(isGLContextActive());
	SOGLU_ASSERT ( mInitialized );
	GL_CHECKED_CALL( glBindFramebuffer( GL_FRAMEBUFFER, mFrameBufferObject ) );

	GL_CHECKED_CALL( glBindTexture ( GL_TEXTURE_2D, mColorTexture ) );
	GL_CHECKED_CALL( glBindRenderbuffer( GL_RENDERBUFFER, mDepthBuffer ) );

	GL_CHECKED_CALL( glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, aWidth, aHeight ) );
	GL_CHECKED_CALL( glBindRenderbuffer( GL_RENDERBUFFER, 0 ) );

	//GL_CHECKED_CALL(glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE ));
	GL_CHECKED_CALL(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP ));
	GL_CHECKED_CALL(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP ));
	GL_CHECKED_CALL(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ));
	GL_CHECKED_CALL(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ));
	GL_CHECKED_CALL( glTexImage2D(
				GL_TEXTURE_2D,
				0,
				aInternalFormat,
				aWidth,
				aHeight,
				0,
				GL_RGBA,
				GL_FLOAT,
				NULL
				) );
	GL_CHECKED_CALL( glBindTexture ( GL_TEXTURE_2D, 0 ) );

	GL_CHECKED_CALL( glFramebufferRenderbuffer(
				GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT,
				GL_RENDERBUFFER,
				mDepthBuffer
				) );

	GL_CHECKED_CALL( glFramebufferTexture2D(
				GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D,
				mColorTexture,
				0
				) );

	GL_CHECKED_CALL( glBindFramebuffer( GL_FRAMEBUFFER, 0 ) );

	mSize.x = aWidth;
	mSize.y = aHeight;
}

void
FrameBufferObject::Resize( unsigned aWidth, unsigned aHeight)
{
	Resize(aWidth, aHeight, GL_RGBA32F);
}

} /*namespace soglu*/



