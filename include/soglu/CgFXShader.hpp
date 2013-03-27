#pragma once

namespace soglu {

class CgFXShader {
public:

	void
	initialize();

	void
	finalize();

	template<typename TParameterType>
	setParameter(std::string aName, const TParameterType &aValue)


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

};

} //namespace soglu
