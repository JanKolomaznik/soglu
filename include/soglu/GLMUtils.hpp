#pragma once

#include <glm/gtc/type_precision.hpp>
#include <glm/glm.hpp>

namespace soglu {

template< unsigned tDim >
struct GLMDimension;


template<>
struct GLMDimension<2>
{
	typedef glm::fvec2 fvec;
	typedef glm::ivec2 ivec;
};

template<>
struct GLMDimension<3>
{
	typedef glm::fvec3 fvec;
	typedef glm::ivec3 ivec;
};

} //namespace soglu
