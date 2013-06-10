#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/ext.hpp>

#include <GL/glew.h>
#include <Cg/cg.h>    /* Can't include this?  Is Cg Toolkit installed! */
#include <Cg/cgGL.h>
#include <string>
#include <map>

#include <ostream>


#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/function.hpp>
#include <boost/graph/graph_concepts.hpp>


#include "soglu/GLViewSetup.hpp"
#include "soglu/BoundingBox.hpp"
#include "soglu/Primitives.hpp"
#include "soglu/OGLTools.hpp"
#include "soglu/GLTextureImage.hpp"

#define SOGLU_D_PRINT(ARG)
#define SOGLU_LOG(ARG)


class RAII : private boost::noncopyable
{
public:
	template< typename TAcquisition, typename TRelease >
	RAII( TAcquisition aAcquisition, TRelease aRelease, bool aAcquire = true ): mAcquisition( aAcquisition ), mRelease( aRelease ), mAcquired( false )
	{ 
		if ( aAcquire ) {
			acquire();
		}
		
	}
	
	~RAII()
	{
		release();
	}
	
	void
	acquire()
	{
		if( ! mAcquired ) {
			mAcquisition();
			mAcquired = true;
		}
	}
	
	void
	release()
	{
		if( mAcquired ) {
			mRelease();
			mAcquired = false;
		}
	}
	
protected:
	boost::function< void() > mAcquisition;
	boost::function< void() > mRelease;
	bool mAcquired;
};

template< typename TResource >
class ResourceGuard : private boost::noncopyable
{
public:
	ResourceGuard( boost::function< TResource() > aAcquisition, boost::function< void(TResource &) > aRelease, bool aAcquire = true ): mAcquisition( aAcquisition ), mRelease( aRelease ), mAcquired( false ), mValid( true )
	{ 
		if ( aAcquire ) {
			acquire();
		}
		
	}

	ResourceGuard(): mAcquired( false ), mValid( false )
	{}
	
	~ResourceGuard()
	{
		release();
	}
	
	void
	acquire()
	{
		if( !mValid ) {
			throw std::string( "acquire() failed. Resource guard not valid" ); //TODO
		}
		if( ! mAcquired ) {
			D_PRINT( "Acquiring resource" );
			mResource = mAcquisition();
			mAcquired = true;
		}
	}
	
	void
	release()
	{
		
		if( mAcquired ) {
			/*if( !mValid ) {
				throw ErrorHandling::EObjectUnavailable( "Resource guard not valid" );
			}*/
			D_PRINT( "Releasing resource" );
			mRelease( mResource );
			mAcquired = false;
		}
	}
	TResource &
	get()
	{ 
		if( !mValid ) {
			throw std::string( "get() failed. Resource guard not valid" );
		}
		if( !mAcquired ) {
			throw std::string( "Resource not acquired!" );
		}
		return mResource; 
	}
protected:

	boost::function< TResource() > mAcquisition;
	boost::function< void(TResource &) > mRelease;
	bool mAcquired;
	bool mValid;

	TResource mResource;

};

template< typename TResource >
boost::shared_ptr< ResourceGuard< TResource > >
makeResourceGuardPtr( boost::function< TResource() > aAcquisition, boost::function< void(TResource &) > aRelease, bool aAcquire = true )
{
	return boost::shared_ptr< ResourceGuard< TResource > >( new ResourceGuard< TResource >( aAcquisition, aRelease, aAcquire ) );
}


//-----------------------------------------------------------------------------------------------------------------

namespace soglu {

extern bool gIsCgInitialized;
extern CGcontext gCgContext;

void
initializeCg();

void
finalizeCg();

class CgException
{
public:
	CgException( std::string aEffectName, std::string aReport ) throw() {}
	CgException( std::string aReport ) throw() {}
	~CgException() throw(){}
};

struct EObjectNotInitialized {};

void 
checkForCgError( const std::string &situation, CGcontext &context = gCgContext );


class CgFXShader 
{
public:

	void
	initialize(const boost::filesystem::path &effectFile);

	void
	finalize();

	template<typename TParameterType>
	void
	setParameter(std::string aName, const TParameterType &aValue);

	template<typename TParameterType>
	void
	setParameter(std::string aName, const TParameterType *aValue, size_t aCount);
	
	//TODO - modify
	void
	setParameter(std::string aName, const GLViewSetup &aViewSetup);
	
	void
	setParameter(std::string aName, const BoundingBox3D &aValue);
	
	void
	setTextureParameter(std::string aName, GLuint aTexture);

	void
	setParameter(std::string aName, const Planef &aPlane);
	
	void
	setParameter(std::string aName, const GLTextureImage &aTexture);

	void
	setParameter(std::string aName, const GLTextureImage3D &aImage);
	/*
	void
	setParameter(std::string aName, const GLTransferFunctionBuffer1D &aTransferFunction);
	*/
	void
	setParameter(std::string aName, const Camera &aCamera);

	template< typename TGeometryRenderFunctor >
	void
	executeTechniquePass( std::string aTechniqueName, TGeometryRenderFunctor aDrawGeometry );
	
	bool
	isInitialized() const 
	{ return mEffectInitialized; }
protected:

	boost::shared_ptr< ResourceGuard< CGeffect > >	mCgEffect;
	std::map< std::string, CGtechnique >	mCgTechniques;
	std::string	mEffectName;

	bool mEffectInitialized;
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
	parameterSetter(CGparameter aParameter, const float *aValue, size_t aSize)
	{
		cgSetParameterValuefr(aParameter, aSize, aValue);
	}

	inline void
	parameterSetter(CGparameter aParameter, const double *aValue, size_t aSize)
	{
		cgSetParameterValuedr(aParameter, aSize, aValue);
	}

	inline void
	parameterSetter(CGparameter aParameter, const int *aValue, size_t aSize)
	{
		cgSetParameterValueir(aParameter, aSize, aValue);
	}
	
	//-------------------------------------------------------------------------

	inline void
	parameterSetter(CGparameter aParameter, const glm::fvec2 &aVec2)
	{
		cgSetParameterValuefr(aParameter, 2, glm::value_ptr(aVec2));
	}

	inline void
	parameterSetter(CGparameter aParameter, const glm::dvec2 &aVec2)
	{
		cgSetParameterValuedr(aParameter, 2, glm::value_ptr(aVec2));
	}
	
	inline void
	parameterSetter(CGparameter aParameter, const glm::ivec2 &aVec2)
	{
		cgSetParameterValueir(aParameter, 2, glm::value_ptr(aVec2));
	}
	
	inline void
	parameterSetter(CGparameter aParameter, const glm::fvec3 &aVec3)
	{
		cgSetParameterValuefr(aParameter, 3, glm::value_ptr( aVec3 ));
	}

	inline void
	parameterSetter(CGparameter aParameter, const glm::dvec3 &aVec3)
	{
		cgSetParameterValuedr(aParameter, 3, glm::value_ptr( aVec3 ));
	}

	inline void
	parameterSetter(CGparameter aParameter, const glm::ivec3 &aVec3)
	{
		cgSetParameterValueir(aParameter, 3, glm::value_ptr(aVec3));
	}
	
	//-------------------------------------------------------------------------
/*	template<size_t tDim>
	inline void
	parameterSetter(CGparameter aParameter, const Vector<float, tDim> &aValue)
	{
		cgSetParameterValuefr(aParameter, tDim, aValue.GetData());
	}
	
	template<size_t tDim>
	inline void
	parameterSetter(CGparameter aParameter, const Vector<int, tDim> &aValue)
	{
		cgSetParameterValueir(aParameter, tDim, aValue.GetData());
	}*/

	template<typename TParameterType>
	inline void
	setCgFXParameter(CGeffect &aEffect, std::string aName, const TParameterType &aValue)
	{
		//SOGLU_ASSERT(isInitialized());
		CGparameter cgParameter = cgGetNamedEffectParameter(aEffect, aName.data() );
		detail::parameterSetter(cgParameter, aValue);
	}


} //namespace detail


template<typename TParameterType>
void
CgFXShader::setParameter(std::string aName, const TParameterType &aValue)
{
	using namespace soglu::detail;
	//SOGLU_ASSERT(isInitialized());

	setCgFXParameter(mCgEffect->get(), aName, aValue);
}

template<typename TParameterType>
void
CgFXShader::setParameter(std::string aName, const TParameterType *aValue, size_t aCount)
{
	//SOGLU_ASSERT(isInitialized());
	CGparameter cgParameter = cgGetNamedEffectParameter( mCgEffect->get(), aName.data() );

	detail::parameterSetter(cgParameter, aValue, aCount);
}

inline void
CgFXShader::setParameter( std::string aName, const GLViewSetup &aViewSetup )
{
	assert(isInitialized());
	setParameter(aName + ".modelViewProj", glm::fmat4x4(aViewSetup.modelViewProj) );
	setParameter(aName + ".modelMatrix", glm::fmat4x4(aViewSetup.model) );
	setParameter(aName + ".projMatrix", glm::fmat4x4(aViewSetup.projection) );
	setParameter(aName + ".viewMatrix", glm::fmat4x4(aViewSetup.view) );
}

inline void
CgFXShader::setParameter( std::string aName, const soglu::Planef &aPlane )
{
	assert(isInitialized());
	setParameter(aName + ".point",aPlane.point());

	setParameter(aName + ".normal", aPlane.normal());
}

inline void
CgFXShader::setParameter( std::string aName, const GLTextureImage &aTexture )
{
	assert(isInitialized());
	setTextureParameter( aName, aTexture.GetTextureGLID() );
}

inline void
CgFXShader::setParameter( std::string aName, const GLTextureImage3D &aImage )
{
	assert(isInitialized());
	setTextureParameter(aName + ".data", aImage.GetTextureGLID() );

	setParameter(aName + ".size", aImage.getExtents().maximum - aImage.getExtents().minimum ); //TODO

	setParameter(aName + ".realSize", aImage.getExtents().realMaximum - aImage.getExtents().realMinimum );

	setParameter(aName + ".realMinimum", aImage.getExtents().realMinimum );

	setParameter(aName + ".realMaximum", aImage.getExtents().realMaximum );
}
/*
inline void
CgFXShader::setParameter(std::string aName, const GLTransferFunctionBuffer1D &aTransferFunction )
{
	assert(isInitialized());
	setTextureParameter(aName + ".data", aTransferFunction.getTextureID() );

	setParameter(aName + ".interval", aTransferFunction.getMappedInterval() );

	setParameter(aName + ".sampleCount", aTransferFunction.getSampleCount() );
}
*/
inline void
CgFXShader::setParameter(std::string aName, const Camera &aCamera)
{
	setParameter(aName + ".eyePosition", aCamera.eyePosition());
	
	setParameter(aName + ".viewDirection", aCamera.targetDirection());
	
	setParameter(aName + ".upDirection", aCamera.upDirection());
}

inline void
CgFXShader::setParameter(std::string aName, const BoundingBox3D &aValue)
{
	assert(isInitialized());
	CGparameter cgParameter = cgGetNamedEffectParameter(mCgEffect->get(), (aName + ".vertices").data());
//	assert( )	TODO check type;

	cgSetParameterValuefr( cgParameter, 3*8, &(aValue.vertices[0].x) );
}

inline void
CgFXShader::setTextureParameter(std::string aName, GLuint aTexture)
{
	assert(isInitialized());
	CGparameter cgParameter = cgGetNamedEffectParameter(mCgEffect->get(), aName.data());
//	assert( )	TODO check type;

	cgGLSetupSampler( cgParameter, aTexture );
	//cgSetSamplerState( cgParameter );
}

template< typename TGeometryRenderFunctor >
void
CgFXShader::executeTechniquePass( std::string aTechniqueName, TGeometryRenderFunctor aDrawGeometry )
{
	if (!isInitialized()) {
		throw EObjectNotInitialized();
	}
	GLPushAtribs pushAttribs; // GL_CHECKED_CALL( glPushAttrib( GL_ALL_ATTRIB_BITS ) );

	
	std::map< std::string, CGtechnique >::iterator it = mCgTechniques.find( aTechniqueName );
	if ( it == mCgTechniques.end() ) {
		throw CgException(mEffectName, std::string("Unavailable technique : ") + aTechniqueName);
	}
	CGtechnique cgTechnique = it->second;

	CGpass pass = cgGetFirstPass( cgTechnique );
	checkForCgError(std::string("getting first pass for technique ") + cgGetTechniqueName(cgTechnique));
	while ( pass ) {
		cgSetPassState( pass );

		aDrawGeometry();

		cgResetPassState( pass );
		pass = cgGetNextPass( pass );
	}
	checkForGLError( "After effect application :" );

	//GL_CHECKED_CALL( glPopAttrib() );
}


typedef CgFXShader CgEffect;

} //namespace soglu

