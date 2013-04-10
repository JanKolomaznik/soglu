#pragma once

#include <ostream>

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
//#include <glm/ext.hpp>

namespace soglu {

struct BoundingBox3D
{
	static const unsigned VertexCount = 8;
	BoundingBox3D( const glm::fvec3 &corner1 = glm::fvec3(), const glm::fvec3 &corner2 = glm::fvec3() )
	{
		vertices[0] = glm::fvec3( corner1 );
		vertices[1] = glm::fvec3( corner2.x, corner1.y, corner1.z );
		vertices[2] = glm::fvec3( corner2.x, corner2.y, corner1.z );
		vertices[3] = glm::fvec3( corner1.x, corner2.y, corner1.z );

		vertices[4] = glm::fvec3( corner1.x, corner1.y, corner2.z );
		vertices[5] = glm::fvec3( corner2.x, corner1.y, corner2.z );
		vertices[6] = glm::fvec3( corner2 );
		vertices[7] = glm::fvec3( corner1.x, corner2.y, corner2.z );
	}

	glm::fvec3
	getMin()const
	{
		return vertices[0];
	}

	glm::fvec3
	getMax()const
	{
		return vertices[6];
	}

	glm::fvec3
	getCenter()const
	{
		return 0.5f * (vertices[0] + vertices[6]);
	}

	glm::fvec3	vertices[VertexCount];
};

/*inline std::ostream &
operator<<( std::ostream & s, const BoundingBox3D &aBBox )
{
//	s << "BBox3d[ " << aBBox.getMin() << "; "  << aBBox.getMin() << " ]";
	return s;
}*/

} //namespace soglu

