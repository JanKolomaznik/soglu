#pragma once

#include <glm/glm.hpp>

struct CameraTraits
{
	typedef glm::vec4 Position;
	typedef glm::vec4 Direction;
};

template<typename TTraits = CameraTraits>
class PerspectiveCamera
{
public:

protected:
	typename TTraits::Direction mUpDirection;
	typename TTraits::Direction mRightDirection;
	typename TTraits::Direction mTargetDirection;
};
