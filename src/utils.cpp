#include <GL/glew.h>
#include <soglu/utils.hpp>
#include <soglu/ErrorHandling.hpp>

namespace soglu {

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