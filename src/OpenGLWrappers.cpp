#include "soglu/OpenGLWrappers.hpp"
#include <GL/glew.h>
#include "soglu/ErrorHandling.hpp"
namespace soglu
{
namespace gl {

namespace {
int mapTextureTarget(TextureTarget aTextureTarget) {
	switch (aTextureTarget) {
	case TextureTarget::Texture1D: return GL_TEXTURE_1D;
	case TextureTarget::Texture2D: return GL_TEXTURE_2D;
	case TextureTarget::Texture3D: return GL_TEXTURE_3D;
	default: SOGLU_ASSERT(false);
	}
	return -1;
}

}  // anonymous namespace

void
activeTexture(TextureUnitId aTextureUnit)
{
	GL_CHECKED_CALL(glActiveTexture(GL_TEXTURE0 + aTextureUnit.value));
}

void
bindTexture(TextureTarget aTextureTarget, TextureId aTexture)
{
	GL_CHECKED_CALL(glBindTexture(mapTextureTarget(aTextureTarget), aTexture.value));
}

void
bindTexture(TextureUnitId aTextureUnit, TextureTarget aTextureTarget, TextureId aTexture)
{
	activeTexture(aTextureUnit);
	bindTexture(aTextureTarget, aTexture);
}

template<>
GLSLShaderId createShader<VERTEX_SHADER_TAG>()
{
	GLSLShaderId id = 0;
	GL_CHECKED_CALL(id = glCreateShader(GL_VERTEX_SHADER));
	return id;
}

template<>
GLSLShaderId createShader<FRAGMENT_SHADER_TAG>()
{
	GLSLShaderId id = 0;
	GL_CHECKED_CALL(id = glCreateShader(GL_FRAGMENT_SHADER));
	return id;
}

template<>
GLSLShaderId createShader<GEOMETRY_SHADER_TAG>()
{
	GLSLShaderId id = 0;
	GL_CHECKED_CALL(id = glCreateShader(GL_GEOMETRY_SHADER));
	return id;
}

void attachShader(GLSLProgramId aProgramId, GLSLShaderId aShaderId)
{
	GL_CHECKED_CALL(glAttachShader(aProgramId, aShaderId));
}

void deleteShader(GLSLShaderId aShaderId)
{
	GL_CHECKED_CALL(glDeleteShader(aShaderId));
}

void detachShader(GLSLProgramId aProgramId, GLSLShaderId aShaderId)
{
	GL_CHECKED_CALL(glDetachShader(aProgramId, aShaderId));
}

void shaderSource(GLSLShaderId aShaderId, const std::string &aSource)
{
	const char *buffer = aSource.c_str();
	GL_CHECKED_CALL(glShaderSource(aShaderId, 1, &buffer, NULL));
}

void compileShader(GLSLShaderId aShaderId)
{
	GL_CHECKED_CALL(glCompileShader(aShaderId));
}

GLSLProgramId createProgram()
{
	GLSLProgramId id = 0;
	GL_CHECKED_CALL(id = glCreateProgram());
	return id;
}

void deleteProgram(GLSLProgramId aProgramId)
{
	GL_CHECKED_CALL(glDeleteProgram(aProgramId));
}

void linkProgram(GLSLProgramId aProgramId)
{
	GL_CHECKED_CALL(glLinkProgram(aProgramId));
}

void useProgram(GLSLProgramId aProgramId)
{
	GL_CHECKED_CALL(glUseProgram(aProgramId));
}

void validateProgram(GLSLProgramId aProgramId)
{
	GL_CHECKED_CALL(glValidateProgram(aProgramId));
}

GLSLUniformLocation getUniformLocation(GLSLProgramId aProgramId, const std::string &aName)
{
	GLint id = -1;
	GL_CHECKED_CALL(id = glGetUniformLocation(aProgramId, aName.c_str()));
	if (id == -1) {
		SOGLU_THROW(EGLSLInvalidUniformName());
	}
 	return id;
}

GLSLAttributeLocation getAttribLocation(GLSLProgramId aProgramId, const std::string &aName)
{
	GLint id = -1;
	GL_CHECKED_CALL(id = glGetAttribLocation(aProgramId, aName.c_str()));
	if (id == -1) {
		SOGLU_THROW(EGLSLInvalidUniformName());
	}
 	return id;
}

SOGLU_DEFINE_SET_PROGRAM_UNIFORM(f);
SOGLU_DEFINE_SET_PROGRAM_UNIFORM(i);
SOGLU_DEFINE_SET_PROGRAM_UNIFORM(ui);

SOGLU_DEFINE_SET_PROGRAM_UNIFORM_MATRIX(2);
SOGLU_DEFINE_SET_PROGRAM_UNIFORM_MATRIX(3);
SOGLU_DEFINE_SET_PROGRAM_UNIFORM_MATRIX(4);

SOGLU_DEFINE_SET_PROGRAM_UNIFORM_VECTOR(2, f);
SOGLU_DEFINE_SET_PROGRAM_UNIFORM_VECTOR(3, f);
SOGLU_DEFINE_SET_PROGRAM_UNIFORM_VECTOR(4, f);

SOGLU_DEFINE_SET_PROGRAM_UNIFORM_VECTOR(2, i);
SOGLU_DEFINE_SET_PROGRAM_UNIFORM_VECTOR(3, i);
SOGLU_DEFINE_SET_PROGRAM_UNIFORM_VECTOR(4, i);

SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR(2, ui);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR(3, ui);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR(4, ui);

}  // namespace gl


}  // namespace soglu
