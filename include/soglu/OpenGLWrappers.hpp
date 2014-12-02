#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "soglu/ErrorHandling.hpp"
#include "soglu/utils.hpp"

namespace soglu
{

typedef unsigned int GLSLShaderId;
typedef unsigned int GLSLProgramId;
typedef unsigned int GLSLShaderType;
typedef int GLSLUniformLocation;
typedef int GLSLAttributeLocation;

enum class TextureTarget: int {
	Texture1D,
	Texture2D,
	Texture3D
};

struct VERTEX_SHADER_TAG {};
struct FRAGMENT_SHADER_TAG {};
struct GEOMETRY_SHADER_TAG {};

struct EGLSLInvalidUniformName : GLException {};
struct EGLSLShaderCompileError : GLException {};
struct EGLSLProgramLinkError : GLException {};
struct EGLSLProgramValidationError : GLException {};


namespace gl {

void activeTexture(TextureUnitId aTextureUnit);
void bindTexture(TextureTarget aTextureTarget, TextureId aTexture);
void bindTexture(TextureUnitId aTextureUnit, TextureTarget aTextureTarget, TextureId aTexture);

template<typename TShaderTypeTag> GLSLShaderId createShader();// { static_assert(false, "Unsupported GLSL shader type."); }
template<> GLSLShaderId createShader<VERTEX_SHADER_TAG>();
template<> GLSLShaderId createShader<FRAGMENT_SHADER_TAG>();
template<> GLSLShaderId createShader<GEOMETRY_SHADER_TAG>();

void attachShader(GLSLProgramId aProgramId, GLSLShaderId aShaderId);
void deleteShader(GLSLShaderId aShaderId);
void detachShader(GLSLProgramId aProgramId, GLSLShaderId aShaderId);
void shaderSource(GLSLShaderId aShaderId, const std::string &aSource);
void compileShader(GLSLShaderId aShaderId);

GLSLProgramId createProgram();
void deleteProgram(GLSLProgramId aProgramId);
void linkProgram(GLSLProgramId aProgramId);
void useProgram(GLSLProgramId aProgramId);
void validateProgram(GLSLProgramId aProgramId);

GLSLUniformLocation getUniformLocation(GLSLProgramId aProgramId, const std::string &aName);
GLSLAttributeLocation getAttribLocation(GLSLProgramId aProgramId, const std::string &aName);

#define SOGLU_SUFFIX_TO_TYPE_f float
#define SOGLU_SUFFIX_TO_TYPE_i int
#define SOGLU_SUFFIX_TO_TYPE_ui unsigned int

#define SOGLU_DECLARE_SET_PROGRAM_UNIFORM(aSuffix) \
	void setProgramUniform(GLSLProgramId aProgramId, GLSLUniformLocation aUniformLocation, const SOGLU_SUFFIX_TO_TYPE_##aSuffix &aValue)

#define SOGLU_DEFINE_SET_PROGRAM_UNIFORM(aSuffix) \
	SOGLU_DECLARE_SET_PROGRAM_UNIFORM(aSuffix) \
	{ GL_CHECKED_CALL(glProgramUniform1##aSuffix(aProgramId, aUniformLocation, aValue)); }

#define SOGLU_DECLARE_SET_PROGRAM_UNIFORM_MATRIX(aMatrixDimension) \
	void setProgramUniform(GLSLProgramId aProgramId, GLSLUniformLocation aUniformLocation, const glm::mat##aMatrixDimension &aValue)

#define SOGLU_DEFINE_SET_PROGRAM_UNIFORM_MATRIX(aMatrixDimension) \
	SOGLU_DECLARE_SET_PROGRAM_UNIFORM_MATRIX(aMatrixDimension) \
	{ GL_CHECKED_CALL(glProgramUniformMatrix##aMatrixDimension##fv(aProgramId, aUniformLocation, 1, GL_FALSE, glm::value_ptr(aValue))); }

#define SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR(aVectorDimension, aSuffix) \
	void setProgramUniform(GLSLProgramId aProgramId, GLSLUniformLocation aUniformLocation, const glm::detail::tvec##aVectorDimension<SOGLU_SUFFIX_TO_TYPE_##aSuffix> &aValue)

#define SOGLU_DEFINE_SET_PROGRAM_UNIFORM_VECTOR(aVectorDimension, aSuffix) \
	SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR(aVectorDimension, aSuffix) \
	{ GL_CHECKED_CALL(glProgramUniform##aVectorDimension##aSuffix##v(aProgramId, aUniformLocation, 1, glm::value_ptr(aValue))); }



#define SOGLU_DECLARE_SET_PROGRAM_UNIFORM_WITH_COUNT(aSuffix) \
	void setProgramUniform(GLSLProgramId aProgramId, GLSLUniformLocation aUniformLocation, const SOGLU_SUFFIX_TO_TYPE_##aSuffix *aValue, size_t aCount)

#define SOGLU_DEFINE_SET_PROGRAM_UNIFORM_WITH_COUNT(aSuffix) \
	SOGLU_DECLARE_SET_PROGRAM_UNIFORM_WITH_COUNT(aSuffix) \
	{ GL_CHECKED_CALL(glProgramUniform1##aSuffix##v(aProgramId, aUniformLocation, aCount, aValue)); }

#define SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR_WITH_COUNT(aVectorDimension, aSuffix) \
	void setProgramUniform(GLSLProgramId aProgramId, GLSLUniformLocation aUniformLocation, const glm::detail::tvec##aVectorDimension<SOGLU_SUFFIX_TO_TYPE_##aSuffix> *aValue, size_t aCount)

#define SOGLU_DEFINE_SET_PROGRAM_UNIFORM_VECTOR_WITH_COUNT(aVectorDimension, aSuffix) \
	SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR_WITH_COUNT(aVectorDimension, aSuffix) \
	{ GL_CHECKED_CALL(glProgramUniform##aVectorDimension##aSuffix##v(aProgramId, aUniformLocation, aCount, glm::value_ptr(aValue[0]))); }

SOGLU_DECLARE_SET_PROGRAM_UNIFORM(f);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM(i);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM(ui);

SOGLU_DECLARE_SET_PROGRAM_UNIFORM_MATRIX(2);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_MATRIX(3);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_MATRIX(4);

SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR(2, f);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR(3, f);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR(4, f);

SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR(2, i);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR(3, i);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR(4, i);

SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR(2, ui);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR(3, ui);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR(4, ui);

//------------------------------------------------
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_WITH_COUNT(f);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_WITH_COUNT(i);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_WITH_COUNT(ui);

SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR_WITH_COUNT(2, f);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR_WITH_COUNT(3, f);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR_WITH_COUNT(4, f);

SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR_WITH_COUNT(2, i);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR_WITH_COUNT(3, i);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR_WITH_COUNT(4, i);

SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR_WITH_COUNT(2, ui);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR_WITH_COUNT(3, ui);
SOGLU_DECLARE_SET_PROGRAM_UNIFORM_VECTOR_WITH_COUNT(4, ui);

inline void setProgramUniformSampler(GLSLProgramId aProgramId, GLSLUniformLocation aSamplerLocation, int aSampler)
{
	setProgramUniform(aProgramId, aSamplerLocation, aSampler);
}

inline void setProgramUniform(GLSLProgramId aProgramId, GLSLUniformLocation aSamplerLocation, TextureUnitId aTextureUnit)
{
	setProgramUniformSampler(aProgramId, aSamplerLocation, aTextureUnit.value);
}

}  // namespace gl
}  // namespace soglu
