#pragma once

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <utility>
//#include <tuple>

#include <fstream>
#include <streambuf>

#include <boost/fusion/tuple.hpp>
//#include <boost/fusion/adapted/boost_tuple.hpp>
//#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/filesystem.hpp>
#include <boost/utility.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "soglu/ErrorHandling.hpp"
#include "soglu/GLTextureImage.hpp"
#include "soglu/utils.hpp"
#include "soglu/OpenGLWrappers.hpp"
#include "soglu/GLViewSetup.hpp"
#include "soglu/Camera.hpp"

namespace soglu {

namespace detail {
template<typename TShaderTypeTag>
struct GLSLShaderTypeTraits;

template<>
struct GLSLShaderTypeTraits<VERTEX_SHADER_TAG>
{
	static const int cOrder = 0;
};

template<>
struct GLSLShaderTypeTraits<FRAGMENT_SHADER_TAG>
{
	static const int cOrder = 1;
};

template<>
struct GLSLShaderTypeTraits<GEOMETRY_SHADER_TAG>
{
	static const int cOrder = 2;
};

//TODO other shader types - COMPUTE, TESSELATION, ...

struct ShaderListDetacher {
	ShaderListDetacher(GLSLProgramId aProgramId): mProgramId(aProgramId) {}
	GLSLProgramId mProgramId;

	template<typename TList>
	void operator()(TList &shader_list) const
	{
		for (auto &shader : shader_list) {
			try {
				gl::detachShader(mProgramId, shader->id());
			} catch (GLException &) {
				// ignore problems
			}
		}
	}
};

struct ShaderListDeleter {
	template<typename TList>
	void operator()(TList &shader_list) const
	{
		shader_list.clear();
	}
};

}  // namespace detail

void
checkForShaderCompileError(GLSLShaderId aShaderId);

void
checkForShaderProgramLinkError(GLSLProgramId aProgramId);

void
checkForShaderProgramValidationError(GLSLProgramId aProgramId);

std::string
loadFile(boost::filesystem::path filename);

template<typename TShaderTypeTag>
class GLSLShader : boost::noncopyable {
public:
	typedef std::shared_ptr<GLSLShader<TShaderTypeTag>> Ptr;
	typedef TShaderTypeTag Tag;


	GLSLShader(const std::string &aSource)
		: mShaderId(0)
	{
		mShaderId = gl::createShader<TShaderTypeTag>();

		gl::shaderSource(mShaderId, aSource);
		gl::compileShader(mShaderId);

		checkForShaderCompileError(mShaderId);
	}

	GLSLShader(GLSLShader &&aShader)
		: mShaderId(aShader.mShaderId)
	{
		aShader.mShaderId = 0;
	}

	virtual
	~GLSLShader()
	{
		if (mShaderId) {
			try {
				SOGLU_ASSERT(isGLContextActive());
				gl::deleteShader(mShaderId);
			} catch (GLException &) {
				SOGLU_LOG("Problem during shader deletion.");
			}
		}
	}

	GLSLShaderId
	id() const
	{
		return mShaderId;
	}

protected:
	GLSLShaderId mShaderId;
};

typedef GLSLShader<VERTEX_SHADER_TAG> GLSLVertexShader;
typedef GLSLShader<FRAGMENT_SHADER_TAG> GLSLFragmentShader;
typedef GLSLShader<GEOMETRY_SHADER_TAG> GLSLGeometryShader;

class GLSLProgram {
public:
	GLSLProgram(bool aInit = false)
		: mProgramId(0)
	{
		if (aInit) {
			initialize();
		}
	}

	GLSLProgram(GLSLProgram &&aProgram)
		: mProgramId(aProgram.mProgramId),
		mAttachedShaders(std::move(aProgram.mAttachedShaders))
	{
		aProgram.mProgramId = 0;
	}

	void
	initialize()
	{
		mProgramId = gl::createProgram();
	}

	void
	finalize()
	{
		if (mProgramId) {
			SOGLU_ASSERT(isGLContextActive());
			boost::fusion::for_each(mAttachedShaders, detail::ShaderListDetacher(mProgramId));
			boost::fusion::for_each(mAttachedShaders, detail::ShaderListDeleter());
			gl::deleteProgram(mProgramId);
			mProgramId = 0;
		}
	}

	virtual
	~GLSLProgram()
	{
		finalize();
	}

	GLSLProgram &
	operator=(GLSLProgram &&aProgram)
	{
		mProgramId = aProgram.mProgramId;
		aProgram.mProgramId = 0;
		mAttachedShaders = std::move(aProgram.mAttachedShaders);
		return *this;
	}

	template<typename TShader>
	void
	attachShader(std::shared_ptr<TShader> aShader)
	{
		gl::attachShader(mProgramId, aShader->id());
		boost::fusion::get<detail::GLSLShaderTypeTraits<typename TShader::Tag>::cOrder>(mAttachedShaders).push_back(aShader);
	}

	void
	link()
	{
		gl::linkProgram(mProgramId);
		checkForShaderProgramLinkError(mProgramId);
	}

	void
	validate()
	{
		gl::validateProgram(mProgramId);
		checkForShaderProgramValidationError(mProgramId);
	}

	template<typename TCallable>
	void
	use(TCallable aCallable)
	{
		bind();
		aCallable();
		unbind();
	}

	void
	bind()
	{
		gl::useProgram(mProgramId);
	}

	void
	unbind()
	{
		gl::useProgram(0);
	}

	GLSLProgramId
	id() const
	{
		return mProgramId;
	}

	GLSLUniformLocation
	getUniformLocation(const std::string &aUniformName)
	{
		return gl::getUniformLocation(mProgramId, aUniformName);
	}

	GLSLAttributeLocation
	getAttributeLocation(const std::string &aAttributeName)
	{
		return gl::getAttribLocation(mProgramId, aAttributeName);
	}

	template<typename TValue>
	void
	setUniform(GLSLUniformLocation aUniformLocation, const TValue &aValue)
	{
		// Allow ADL
		using namespace soglu::gl;
		setProgramUniform(mProgramId, aUniformLocation, aValue);
	}

	template<typename TValue>
	void
	setUniformByName(const std::string &aUniformName, const TValue &aValue)
	{
		try {
		setUniform(getUniformLocation(aUniformName), aValue);
		} catch (EGLSLInvalidUniformName &) {

		}
	}

	void
	setUniformByName(const std::string &aUniformName, const GLTextureImageTyped<3> &aImage, TextureUnitId aTextureUnit)
	{
		gl::bindTexture(aTextureUnit, soglu::TextureTarget::Texture3D, aImage.GetTextureGLID());
		setUniformByName(aUniformName + ".data", aTextureUnit);
		setUniformByName(aUniformName + ".size", aImage.getExtents().maximum - aImage.getExtents().minimum ); //TODO
		setUniformByName(aUniformName + ".realSize", aImage.getExtents().realMaximum - aImage.getExtents().realMinimum );
		setUniformByName(aUniformName + ".realMinimum", aImage.getExtents().realMinimum );
		setUniformByName(aUniformName + ".realMaximum", aImage.getExtents().realMaximum );
	}

	void
	setUniformByName(const std::string &aUniformName, const soglu::GLViewSetup &aViewSetup)
	{
		setUniformByName(aUniformName + ".modelViewProj", glm::fmat4x4(aViewSetup.modelViewProj) );
		setUniformByName(aUniformName + ".modelMatrix", glm::fmat4x4(aViewSetup.model) );
		setUniformByName(aUniformName + ".projMatrix", glm::fmat4x4(aViewSetup.projection) );
		setUniformByName(aUniformName + ".viewMatrix", glm::fmat4x4(aViewSetup.view) );
	}

	void
	setUniformByName(const std::string &aUniformName, const PerspectiveCamera &aCamera)
	{
		setUniformByName(aUniformName + ".eyePosition", aCamera.eyePosition());
		setUniformByName(aUniformName + ".viewDirection", aCamera.targetDirection());
		setUniformByName(aUniformName + ".upDirection", aCamera.upDirection());
	}

protected:
	GLSLProgramId mProgramId;

	boost::fusion::tuple<
		std::vector<GLSLShader<VERTEX_SHADER_TAG>::Ptr>,
		std::vector<GLSLShader<FRAGMENT_SHADER_TAG>::Ptr>,
		std::vector<GLSLShader<GEOMETRY_SHADER_TAG>::Ptr>> mAttachedShaders;
};

GLSLProgram createGLSLProgramFromVertexAndFragmentShader(const boost::filesystem::path &aVertexShader, const boost::filesystem::path &aFragmentShader);

void testRender(GLSLProgram &aProgram, float aAspectRatio);

}  // namespace soglu
