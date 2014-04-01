#include <GL/glew.h>
#include <soglu/utils.hpp>
#include <soglu/ErrorHandling.hpp>

namespace soglu {

#define SOGLU_DEFINE_GLOBJECT_INITIALIZATION(ObjectClass, aFunction) \
	void ObjectClass::initialize() { GL_CHECKED_CALL(aFunction(1, &value)); }

#define SOGLU_DEFINE_GLOBJECT_FINALIZATION(ObjectClass, aFunction) \
	void ObjectClass::finalize() { GL_CHECKED_CALL(aFunction(1, &value)); }

#define SOGLU_DEFINE_GLOBJECT_METHODS(ObjectClass, aGenFunction, aDeleteFunction)\
	SOGLU_DEFINE_GLOBJECT_INITIALIZATION(ObjectClass, aGenFunction)\
	SOGLU_DEFINE_GLOBJECT_FINALIZATION(ObjectClass, aDeleteFunction)

SOGLU_DEFINE_GLOBJECT_METHODS(TextureObject, glGenTextures, glDeleteTextures)
SOGLU_DEFINE_GLOBJECT_METHODS(FramebufferObject, glGenFramebuffers, glDeleteFramebuffers)
SOGLU_DEFINE_GLOBJECT_METHODS(RenderbufferObject, glGenRenderbuffers, glDeleteRenderbuffers)
SOGLU_DEFINE_GLOBJECT_METHODS(BufferObject, glGenBuffers, glDeleteBuffers)
SOGLU_DEFINE_GLOBJECT_METHODS(VertexArrayObject, glGenVertexArrays, glDeleteVertexArrays)


void
TextureObject::bind(uint32_t aTextureTarget)
{
	SOGLU_ASSERT(*this);
	GL_CHECKED_CALL(glBindTexture(aTextureTarget, value));
}

void
TextureObject::unbind(uint32_t aTextureTarget)
{
	GL_CHECKED_CALL(glBindTexture(aTextureTarget, 0));
}

void
FramebufferObject::bind()
{
	SOGLU_ASSERT(*this);
	GL_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, value));
}

void
FramebufferObject::unbind()
{
	GL_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void
RenderbufferObject::bind()
{
	SOGLU_ASSERT(*this);
	GL_CHECKED_CALL(glBindRenderbuffer(GL_RENDERBUFFER, value));
}

void
RenderbufferObject::unbind()
{
	GL_CHECKED_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void
BufferObject::bind(uint32_t aBufferType)
{
	SOGLU_ASSERT(*this);
	GL_CHECKED_CALL(glBindBuffer(aBufferType, value));
}

void
BufferObject::unbind(uint32_t aBufferType)
{
	GL_CHECKED_CALL(glBindBuffer(aBufferType, 0));
}

void
VertexArrayObject::bind()
{
	SOGLU_ASSERT(*this);
	GL_CHECKED_CALL(glBindVertexArray(value));
}

void
VertexArrayObject::unbind()
{
	GL_CHECKED_CALL(glBindVertexArray(0));
}

Sampler::Sampler(bool aInit)
{
	if (aInit) {
		initialize();
	}
}

Sampler::~Sampler()
{
	if (*this) {
		finalize();
	}
}

void
Sampler::initialize()
{
	GL_CHECKED_CALL(glGenSamplers(1, &value));
}

void
Sampler::finalize()
{
	GL_CHECKED_CALL(glDeleteSamplers(1, &value));
}

void
Sampler::bind(const TextureUnitId &aTextureUnit)
{
	GL_CHECKED_CALL(glBindSampler(aTextureUnit.value, value));
}

void
Sampler::unbind(const TextureUnitId &aTextureUnit)
{
	GL_CHECKED_CALL(glBindSampler(aTextureUnit.value, 0));
}

void
Sampler::setParameter(int aParameter, int aParameterValue)
{
	GL_CHECKED_CALL(glSamplerParameteri(value, aParameter, aParameterValue));
}

} // namespace soglu
