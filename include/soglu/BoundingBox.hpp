#pragma once

#include <ostream>

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
//#include <glm/ext.hpp>

#include <soglu/Primitives.hpp>

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

void
getBBoxMinMaxDistance( 
			const BoundingBox3D	&bbox, 
			const glm::fvec3 	&eyePoint, 
			const glm::fvec3  	&direction, 
			float 			&min, 
			float 			&max,
		       	unsigned		&minId,
		       	unsigned		&maxId
			);


unsigned
getPlaneVerticesInBoundingBox( 
		const soglu::BoundingBox3D		&bbox, 
		const glm::fvec3 	&planePoint, 
		const glm::fvec3 	&planeNormal,
		unsigned			minId,
	       	glm::fvec3 		vertices[]
		);

unsigned
getPlaneVerticesInBoundingBox( 
		const soglu::BoundingBox3D		&bbox, 
		const soglu::Planef			&plane,
		unsigned			minId,
	       	glm::fvec3 		vertices[]
		);

unsigned
getPlaneVerticesInBoundingBox( 
		const soglu::BoundingBox3D		&bbox, 
		const soglu::Planef			&plane,
	       	glm::fvec3 		vertices[]
		);

/*size_t
fillPlaneBBoxIntersectionBufferFill(
		const BoundingBox3D	&bbox,
		const Camera		&camera,
		unsigned 		numberOfSteps,
		Vector3f		*vertices,
		unsigned		*indices,
		float			cutPlane,
		unsigned		primitiveRestart
		);*/


} //namespace soglu

