#pragma once

#include <glm/glm.hpp>
#include "soglu/ACamera.h"
#include <glm/gtc/quaternion.hpp>

namespace soglu {

struct CameraTraits
{
	typedef glm::vec4 Position;
	typedef glm::vec4 Direction;
};

template<typename TTraits = CameraTraits>
class PerspectiveCamera: public ACamera
{
public:
	
	void
	setFieldOfView(float aAngle)
	{
		mFieldOfViewY = aAngle;
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

typedef PerspectiveCamera<> Camera;
typedef PerspectiveCamera<> OrthoCamera;

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

} //namespace soglu
