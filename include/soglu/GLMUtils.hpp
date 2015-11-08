#pragma once

#include <glm/gtc/type_precision.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <cassert>

template<typename T>
inline std::ostream &
operator<< (std::ostream &out, const glm::tvec3<T> &vec) {
    out << glm::to_string(vec);
    return out;
}


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

template< typename CoordType >
glm::tvec2< CoordType >
purgeDimension( const glm::tvec3< CoordType > &u, size_t purgedDimension = 2 )
{
	glm::tvec2< CoordType > data;
	size_t j = 0;
	for( size_t i=0; i < 3; ++i ) {
		if( i != purgedDimension ) {
			data[j++] = u[i];
		}
	}
	return data;
}

template< typename CoordType >
glm::tvec3< CoordType >
insertDimension( const glm::tvec2< CoordType > &u, CoordType value, size_t insertedDimension = 2 )
{

	glm::tvec3< CoordType > data;
	size_t j = 0;
	for( size_t i=0; i < 3; ++i ) {
		if( i != insertedDimension ) {
			data[i] = u[j++];
		} else {
			data[i] = value;
		}
	}
	return data;
}

template <typename TCoord>
void set(glm::tvec2<TCoord> &aV, const TCoord *aData)
{
	assert(aData);
	for (size_t i = 0; i < 2; ++i) {
		aV[i] = aData[i];
	}
}

template <typename TCoord>
void set(glm::tvec3<TCoord> &aV, const TCoord *aData)
{
	assert(aData);
	for (size_t i = 0; i < 3; ++i) {
		aV[i] = aData[i];
	}
}

} //namespace soglu
