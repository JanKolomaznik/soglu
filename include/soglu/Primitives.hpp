#pragma once


#include <glm/glm.hpp>
//#include <glm/ext.hpp>

namespace soglu {

/**
 * Basic space planes, each constant also defines index of axis perpendicular
 * to given plane.
 **/
enum CartesianPlanes{
	YZ_PLANE = 0,
	XZ_PLANE = 1,
	XY_PLANE = 2
};


template< typename TVector >
class Plane
{
public:
	typedef TVector Vec;
	typedef TVector Point;

	Plane()
	{}
	Plane( const Point &aPoint, const Vec &aNormal ): mPoint( aPoint ), mNormal( aNormal )
	{}

	/*Plane( const Vec &aPoint, const Vec &aNormal ): mPoint( aPoint ), mNormal( aNormal )
	{}*/

	Point&
	point()
	{ return mPoint; }

	const Point&
	point()const
	{ return mPoint; }

	Vec&
	normal()
	{ return mNormal; }

	const Vec&
	normal()const
	{ return mNormal; }
protected:
	Point mPoint;
	Vec mNormal;
};

typedef Plane<glm::fvec3> Planef;

} //namespace soglu


