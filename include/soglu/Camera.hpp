#pragma once

#include <glm/glm.hpp>
#include <soglu/ACamera.h>
#include <soglu/GLMUtils.hpp>
#include <glm/gtc/quaternion.hpp>
#include <soglu/GLViewSetup.hpp>

namespace soglu {

struct CameraTraits
{
	typedef glm::vec4 Position;
	typedef glm::vec4 Direction;
};

//template<typename TTraits = CameraTraits>
class PerspectiveCamera: public ACamera
{
public:

	void
	setFieldOfView(float aAngle)
	{
		mFieldOfViewY = aAngle;
	}

	float
	fieldOfView() const
	{
		return mFieldOfViewY;
	}

	void
	moveTo(glm::fvec3 aPos)
	{
		mEyePos = aPos;
	}

	void
	move(glm::fvec3 aMoveVector)
	{
		mEyePos += aMoveVector;
	}

	void
	rotate(glm::fquat aQ)
	{
		mUpDirection = glm::normalize(aQ * mUpDirection);
		mTargetDirection = glm::normalize(aQ * mTargetDirection);

		updateRightDirection();
	}


protected:
	//typename TTraits::Direction mUpDirection;
	//typename TTraits::Direction mRightDirection;
	//typename TTraits::Direction mTargetDirection;

	float mFieldOfViewY;
};

typedef PerspectiveCamera Camera;


class OrthoCamera: public ACamera
{
public:
	void
	setWindow( FloatType aWidth, FloatType aHeight )
	{
		mTopLeftCorner = glm::fvec2(-0.5f*aWidth, 0.5f*aHeight);
		mBottomRightCorner = glm::fvec2(0.5f*aWidth, -0.5f*aHeight);
		/*mLeft = -0.5f*aWidth;
		mRight = 0.5f*aWidth;
		mTop = 0.5f*aHeight;
		mBottom = -0.5f*aHeight;*/
	}

	glm::fvec2
	topLeftCorner()const
	{
		return mTopLeftCorner;
	}

	glm::fvec2
	bottomRightCorner()const
	{
		return mBottomRightCorner;
	}
protected:
	glm::fvec2 mTopLeftCorner;
	glm::fvec2 mBottomRightCorner;
	/*float mLeft;
	float mRight;
	float mTop;
	float mBottom;*/
};

//typedef PerspectiveCamera<> OrthoCamera;

template<typename TCamera>
void
cameraRotateAroundPoint(TCamera &aCamera, glm::fquat aQ, glm::fvec3 aPoint)
{
	glm::fvec3 pos = aCamera.eyePosition();
	glm::fvec3 tmpPos = pos - aPoint;

	pos = (aQ * tmpPos) + aPoint;

	aCamera.rotate(aQ);
	aCamera.moveTo(pos);

	/*

	Position direction = toGLM(RotatePoint(fromGLM(mTargetDirection), q));
	direction = glm::normalize(direction);
	Position dist = -mTargetDistance * direction;

	mEyePos = mTargetPos + dist;
	mUpDirection = toGLM(RotatePoint(fromGLM(mUpDirection), q ));
	mUpDirection = glm::normalize(mUpDirection);

	mTargetDirection = direction;
	mRightDirection = glm::cross(mTargetDirection, mUpDirection);*/


}

template<typename TCamera>
void
cameraYawPitchAroundPoint(TCamera &aCamera, glm::fvec2 aYawPitch, glm::fvec3 aPoint)
{
	glm::fquat q = glm::angleAxis(aYawPitch[0], aCamera.upDirection()) * glm::angleAxis(aYawPitch[1], aCamera.rightDirection());

	cameraRotateAroundPoint(aCamera, q, aPoint);
}

template<typename TCamera>
void
dollyCamera(TCamera &aCamera, glm::fvec3 aMoveVector)
{
	aCamera.move(aMoveVector);
}

template <typename TType>
glm::tvec3<TType>
getDirectionFromScreenCoordinatesAndCameraPosition(const glm::tvec2<TType> &aScreenCoords, const GLViewSetup &aViewSetup, const glm::tvec3<TType> &aCameraPos )
{
	glm::tvec3<TType> tmp(glm::unProject(
		glm::dvec3(aScreenCoords, 0.0),
		aViewSetup.modelView,
		aViewSetup.projection,
		aViewSetup.viewport
	));
	//std::cout << "aViewSetup.modelView = " << glm::to_string(aViewSetup.modelView) << std::endl;
	//std::cout << "aViewSetup.projection = " << glm::to_string(aViewSetup.projection) << std::endl;
	//std::cout << "aViewSetup.viewport = " << glm::to_string(aViewSetup.viewport) << std::endl;
	glm::tvec3<TType> direction;
	/*GLint res = gluUnProject(
			aScreenCoords[0],
			aScreenCoords[1],
			0.0,
			aViewSetup.model,
			aViewSetup.proj,
			aViewSetup.view,
			&(objCoords1[0]),
			&(objCoords1[1]),
			&(objCoords1[2])
			);
	if( res == GLU_FALSE ) {
		_THROW_ GLException( "Cannot unproject screen coordinates" );
	}*/

	//LOG( "screen : " << aScreenCoords );
	//LOG( "coords1 : " << objCoords1 );
	direction = tmp - aCameraPos;
	//std::cout << "TMP = " << glm::to_string(tmp) << std::endl;
	//std::cout << "aCameraPos = " << glm::to_string(aCameraPos) << std::endl;
	//std::cout << "direction = " << glm::to_string(tmp) << std::endl;
	glm::normalize(direction);
	return direction;
}

glm::dvec3
getPointFromScreenCoordinates(glm::fvec2 aScreenCoords, const GLViewSetup &aViewSetup, double aZValue = 0.0);

} //namespace soglu
