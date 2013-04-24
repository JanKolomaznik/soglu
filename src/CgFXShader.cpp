#include "soglu/CgFXShader.hpp"
#include <boost/bind.hpp>
#include <boost/format.hpp>


namespace soglu
{

bool gIsCgInitialized = false;
CGcontext gCgContext;

void
initializeCg()
{//TODO - check if design with global variables is proper one
	if ( gIsCgInitialized ) {
		LOG( "Cg already initialized" );
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
	LOG( "Cg initialized" );
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
	mCgEffect = makeResourceGuardPtr< CGeffect >( boost::bind<CGeffect>( &cgCreateEffectFromFile, gCgContext, effectFile.string().data(), static_cast<const char **>(0) ), boost::bind<void>( &cgDestroyEffect, _1 ) );

	checkForCgError(boost::str(boost::format("creating cg effect from file \"%1%\".") %  effectFile));

	LOG( "Cg effect \"" << effectFile.filename() << "\" loaded" );

	CGtechnique cgTechnique = cgGetFirstTechnique(mCgEffect->get());
	if ( !cgTechnique ) {
		throw CgException( mEffectName, "No technique found!" );
	}
	while (cgTechnique) {
		if ( cgValidateTechnique(cgTechnique) == CG_FALSE ) {
			LOG( "\tTechnique " << cgGetTechniqueName(cgTechnique) << " did not validate. Skipping." );
		} else {

			LOG( "\tTechnique " << cgGetTechniqueName(cgTechnique) << " validated. Enabling." );
			mCgTechniques[ cgGetTechniqueName(cgTechnique) ] = cgTechnique;
		}
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
checkForCgError( const std::string &situation, CGcontext &context  )
{
	CGerror error = cgGetError();
	if (error != CG_NO_ERROR) {
		std::string errString;
		const char *msg = cgGetLastErrorString(&error);
		if (msg) {
			errString = msg;
		}
		
		std::string message = situation + errString;
		const char *listing = cgGetLastListing(context);
		if( listing ) {
			message += std::string("\nLast listing:") + std::string(listing);
		}
		throw CgException( message );
	}
}

} //namespace soglu


