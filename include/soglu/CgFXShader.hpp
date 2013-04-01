#pragma once

#include <Cg/cg.h>    /* Can't include this?  Is Cg Toolkit installed! */
#include <Cg/cgGL.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <string>

namespace soglu {

class CgFXShader 
{
public:

	void
	initialize();

	void
	finalize();

	template<typename TParameterType>
	void
	setParameter(std::string aName, const TParameterType &aValue);

	template<typename TParameterType>
	void
	setParameter(std::string aName, const TParameterType *aValue, size_t aCount);

/*
	template< size_t Dim >
	void
	setParameter( std::string aName, const Vector<float, Dim> &value );

	template< size_t Dim >
	void
	setParameter( std::string aName, const Vector<double, Dim> &value );

	template< size_t Dim >
	void
	setParameter( std::string aName, const Vector<unsigned int, Dim> &value );

	template< size_t Dim >
	void
	setParameter( std::string aName, const Vector<int, Dim> &value );

	void
	setParameter( std::string aName, const GLTextureImage &aTexture );

	void
	setParameter( std::string aName, const GLTextureImage3D &aImage );

	void
	setParameter( std::string aName, const GLTransferFunctionBuffer1D &aTransferFunction );

	void
	setParameter( std::string aName, float aValue );

	void
	setParameter( std::string aName, double aValue );

	void
	setParameter( std::string aName, int aValue );

	void
	setParameter( std::string aName, const float *aValue, size_t aCount );

	void
	setParameter( std::string aName, const double *aValue, size_t aCount );

	void
	setParameter( std::string aName, const int *aValue, size_t aCount );
	
	void
	setParameter( std::string aName, const glm::fmat4x4 &aMatrix );
	
	void
	setParameter( std::string aName, const glm::dmat4x4 &aMatrix );

	void
	setParameter( std::string aName, const BoundingBox3D &aValue );
	
	void
	SetTextureParameter( std::string aName, GLuint aTexture );

	void
	setParameter( std::string aName, const M4D::Planef &aPlane );
	
	void
	setParameter( std::string aName, const M4D::GLViewSetup &aViewSetup );

	void
	setGLStateMatrixParameter( std::string aName, CGGLenum matrix, CGGLenum transform );
*/
protected:
	std::string	mEffectName;	

};


namespace detail {


	inline void
	parameterSetter(CGparameter aParameter, float aValue)
	{
		cgSetParameterValuefr(aParameter, 1, &aValue);
	}

	inline void
	parameterSetter(CGparameter aParameter, double aValue)
	{
		cgSetParameterValuedr(aParameter, 1, &aValue);
	}

	inline void
	parameterSetter(CGparameter aParameter, int aValue)
	{
		cgSetParameterValueir(aParameter, 1, &aValue);
	}


	inline void
	parameterSetter(CGparameter aParameter, const glm::fmat4x4 &aMatrix)
	{
		cgSetParameterValuefr(aParameter, 16, glm::value_ptr( aMatrix ));
	}

	inline void
	parameterSetter(CGparameter aParameter, const glm::dmat4x4 &aMatrix)
	{
		cgSetParameterValuedr(aParameter, 16, glm::value_ptr( aMatrix ));
	}

	//-------------------------------------------------------------------------

	inline void
	parameterSetter(CGparameter aParameter, const float *aValue)
	{
		cgSetParameterValuefr(aParameter, 1, &aValue);
	}

	inline void
	parameterSetter(CGparameter aParameter, const double *aValue)
	{
		cgSetParameterValuedr(aParameter, 1, &aValue);
	}

	inline void
	parameterSetter(CGparameter aParameter, const int *aValue)
	{
		cgSetParameterValueir(aParameter, 1, &aValue);
	}

} //namespace detail


template<typename TParameterType>
void
CgFXShader::setParameter(std::string aName, const TParameterType &aValue)
{
	SOGLU_ASSERT(isInitialized());
	CGparameter cgParameter = cgGetNamedEffectParameter( mCgEffect->get(), aName.data() );

	detail::parameterSetter(cgParameter, aValue);
}

template<typename TParameterType>
void
CgFXShader::setParameter(std::string aName, const TParameterType *aValue, size_t aCount)
{
	SOGLU_ASSERT(isInitialized());
	CGparameter cgParameter = cgGetNamedEffectParameter( mCgEffect->get(), aName.data() );

	detail::parameterSetter(cgParameter, aValue, aCount);
}


} //namespace soglu
