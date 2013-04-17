#pragma once

#include <glm/glm.hpp>
#include "soglu/ACamera.h"

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
	setFieldOfView(float aAngle);

protected:
	//typename TTraits::Direction mUpDirection;
	//typename TTraits::Direction mRightDirection;
	//typename TTraits::Direction mTargetDirection;
};

typedef PerspectiveCamera<> Camera;
typedef PerspectiveCamera<> OrthoCamera;

} //namespace soglu
