#include <GL/glew.h>
#include "soglu/CgFXShader.hpp"
#include <boost/bind.hpp>
#include <boost/format.hpp>

#include "soglu/GLTextureImage.hpp"
#include "soglu/Camera.hpp"
#include "soglu/BoundingBox.hpp"
#include "soglu/GLViewSetup.hpp"

namespace soglu
{

bool gIsCgInitialized = false;
CGcontext gCgContext;

void
initializeCg()
{//TODO - check if design with global variables is proper one
	if ( gIsCgInitialized ) {
		SOGLU_LOG( "Cg already initialized" );
		return;
	}
	gCgContext = cgCreateContext();
	checkForCgError("creating context");
	cgGLSetDebugMode( CG_FALSE );
	cgSetParameterSettingMode(gCgContext, /*CG_IMMEDIATE_PARAMETER_SETTING*/CG_DEFERRED_PARAMETER_SETTING);
	cgGLRegisterStates(gCgContext);
	checkForCgError("registering standard CgFX states");
	cgGLSetManageTextureParameters(gCgContext, CG_TRUE);
	checkForCgError("manage texture parameters");

	gIsCgInitialized = true;
	SOGLU_LOG( "Cg initialized" );
}

void
finalizeCg()
{
	cgDestroyContext( gCgContext );
}

void
CgFXShader::initialize(//CGcontext   				&cgContext,
			const boost::filesystem::path 		&effectFile
			)
{
	mEffectName = effectFile.filename().string();

	if ( !boost::filesystem::is_regular_file( effectFile ) ) {
		throw CgException( mEffectName, boost::str( boost::format( "Effect could not be loaded! `%1%` is not regular file." ) %effectFile ) );
	}
	checkForCgError(boost::str(boost::format("before creating cg effect from file \"%1%\".") %  effectFile));
	mCgEffect = makeResourceGuardPtr< CGeffect >( boost::bind<CGeffect>( &cgCreateEffectFromFile, gCgContext, effectFile.string().data(), static_cast<const char **>(0) ), boost::bind<void>( &cgDestroyEffect, _1 ) );

	checkForCgError(boost::str(boost::format("creating cg effect from file \"%1%\".") %  effectFile));

	SOGLU_LOG( "Cg effect \"" << effectFile.filename() << "\" loaded" );

	CGtechnique cgTechnique = cgGetFirstTechnique(mCgEffect->get());
	if ( !cgTechnique ) {
		throw CgException( mEffectName, "No technique found!" );
	}
	while (cgTechnique) {
		std::string techniqueName = cgGetTechniqueName(cgTechnique);
		checkForCgError(boost::str(boost::format("before validation of technique \"%1%\" from \"%2%\".") % techniqueName % effectFile));
		if ( cgValidateTechnique(cgTechnique) == CG_FALSE ) {
			SOGLU_LOG( "\tTechnique " << techniqueName << " did not validate. Skipping." );
		} else {

			SOGLU_LOG( "\tTechnique " << techniqueName << " validated. Enabling." );
			mCgTechniques[ techniqueName ] = cgTechnique;
		}
		checkForCgError(boost::str(boost::format("after validation of technique \"%1%\" from \"%2%\".") % techniqueName % effectFile));
		cgTechnique = cgGetNextTechnique( cgTechnique );
	}
	if ( mCgTechniques.size() == 0 ) {
		throw CgException( mEffectName, "No technique validated!" );
	}
	mEffectInitialized = true;
}


void
CgFXShader::finalize()
{
	if (mEffectInitialized) {
		mEffectInitialized = false;
		mCgEffect.reset();
	}
}

void
CgFXShader::setParameter( std::string aName, const GLViewSetup &aViewSetup )
{
	assert(isInitialized());
	setParameter(aName + ".modelViewProj", glm::fmat4x4(aViewSetup.modelViewProj) );
	setParameter(aName + ".modelMatrix", glm::fmat4x4(aViewSetup.model) );
	setParameter(aName + ".projMatrix", glm::fmat4x4(aViewSetup.projection) );
	setParameter(aName + ".viewMatrix", glm::fmat4x4(aViewSetup.view) );
}

void
CgFXShader::setParameter( std::string aName, const soglu::Planef &aPlane )
{
	assert(isInitialized());
	setParameter(aName + ".point",aPlane.point());

	setParameter(aName + ".normal", aPlane.normal());
}

void
CgFXShader::setParameter( std::string aName, const GLTextureImage &aTexture )
{
	assert(isInitialized());
	setTextureParameter( aName, aTexture.GetTextureGLID() );
}

void
CgFXShader::setParameter( std::string aName, const GLTextureImageTyped<3> &aImage )
{
	assert(isInitialized());
	setTextureParameter(aName + ".data", aImage.GetTextureGLID() );

	setParameter(aName + ".size", aImage.getExtents().maximum - aImage.getExtents().minimum ); //TODO

	setParameter(aName + ".realSize", aImage.getExtents().realMaximum - aImage.getExtents().realMinimum );

	setParameter(aName + ".realMinimum", aImage.getExtents().realMinimum );

	setParameter(aName + ".realMaximum", aImage.getExtents().realMaximum );
}
/*
void
CgFXShader::setParameter(std::string aName, const GLTransferFunctionBuffer1D &aTransferFunction )
{
	assert(isInitialized());
	setTextureParameter(aName + ".data", aTransferFunction.getTextureID() );

	setParameter(aName + ".interval", aTransferFunction.getMappedInterval() );

	setParameter(aName + ".sampleCount", aTransferFunction.getSampleCount() );
}
*/
void
CgFXShader::setParameter(std::string aName, const PerspectiveCamera &aCamera)
{
	setParameter(aName + ".eyePosition", aCamera.eyePosition());

	setParameter(aName + ".viewDirection", aCamera.targetDirection());

	setParameter(aName + ".upDirection", aCamera.upDirection());
}

void
CgFXShader::setParameter(std::string aName, const BoundingBox3D &aValue)
{
	assert(isInitialized());
	CGparameter cgParameter = cgGetNamedEffectParameter(mCgEffect->get(), (aName + ".vertices").data());
//	assert( )	TODO check type;

	cgSetParameterValuefr( cgParameter, 3*8, &(aValue.vertices[0].x) );
}

void
CgFXShader::setTextureParameter(std::string aName, GLuint aTexture)
{
	assert(isInitialized());
	CGparameter cgParameter = cgGetNamedEffectParameter(mCgEffect->get(), aName.data());
//	assert( )	TODO check type;

	cgGLSetupSampler( cgParameter, aTexture );
	//cgSetSamplerState( cgParameter );
}




void
checkForCgError( const std::string &situation, CGcontext &context  )
{
	CGerror error = cgGetError();
	if (error != CG_NO_ERROR) {
		std::string errString;
		errString += boost::str(boost::format("CG error 0x%02X,") % error);
		const char *msg = cgGetLastErrorString(&error);
		if (msg) {
			errString = msg;
		}

		std::string message = situation + errString;
		const char *listing = cgGetLastListing(context);
		if( listing ) {
			message += std::string("\nLast listing:") + std::string(listing);
		}
		D_PRINT("CG ERROR: " << message << std::endl);
		throw CgException( message );
	}
}

} //namespace soglu


