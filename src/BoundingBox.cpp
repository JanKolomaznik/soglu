#include <soglu/BoundingBox.hpp>
#include <soglu/OGLTools.hpp>
#include <soglu/GLMUtils.hpp>
#include <soglu/GeometricAlgorithms.hpp>
#include <glm/gtx/projection.hpp>

#include <iostream>
namespace soglu
{

static const unsigned edgeOrder[8][12] = {
		{ 10, 11,  9,  4,  8,  5,  1,  0,  6,  2,  3,  7 },
		{ 11,  8, 10,  5,  9,  6,  2,  1,  7,  3,  0,  4 },
		{  8,  9, 11,  6, 10,  7,  3,  2,  4,  0,  1,  5 },
		{  9, 10,  8,  7, 11,  4,  0,  3,  5,  1,  2,  6 },
		{  1,  0,  2,  4,  3,  7, 10, 11,  6,  9,  8,  5 },
		{  2,  1,  3,  5,  0,  4, 11,  8,  7, 10,  9,  6 },
		{  3,  2,  0,  6,  1,  5,  8,  9,  4, 11, 10,  7 },
		{  0,  3,  1,  7,  2,  6,  9, 10,  5,  8, 11,  4 }
	};

static const unsigned edgeVertexAMapping[12] = { 0, 1, 2, 3, 0, 1, 2, 3, 4, 5, 6, 7 };
static const unsigned edgeVertexBMapping[12] = { 1, 2, 3, 0, 4, 5, 6, 7, 5, 6, 7, 4 };

static unsigned
GetBBoxEdgePointA( unsigned idx )
{
	SOGLU_ASSERT( idx < 12 ); //only 12 edges

	/*if( idx < 8 ) {
		return idx % 4;
	}
	return idx - 4;*/
	return edgeVertexAMapping[idx];
}

static unsigned
GetBBoxEdgePointB( unsigned idx )
{
	SOGLU_ASSERT( idx < 12 ); //only 12 edges

	/*if( idx < 4 ) {
		return (idx + 1) % 4;
	}
	if( idx < 8 ) {
		return idx;
	}
	if( idx < 11 ) {
		return idx - 3;
	}
	return 4;*/
	return edgeVertexBMapping[idx];
}

void
getBBoxMinMaxDistance(
			const BoundingBox3D	&bbox,
			const glm::fvec3 	&eyePoint,
			const glm::fvec3 	&direction,
			float 			&min,
			float 			&max,
		       	unsigned			&minId,
		       	unsigned			&maxId
			)
{
	min = glm::length(glm::proj(bbox.vertices[0] - eyePoint, direction));
	minId = 0;
	max = glm::length(glm::proj(bbox.vertices[0] - eyePoint, direction));
	maxId = 0;
	for( unsigned i=1; i<8; ++i ) {
		float tmpSize = glm::length(glm::proj(bbox.vertices[i] - eyePoint, direction));
		if( tmpSize < min ) {
			min = tmpSize;
			minId = i;
		}
		if( tmpSize > max ) {
			max = tmpSize;
			maxId = i;
		}
	}

}

unsigned
getPlaneVerticesInBoundingBox(
		const BoundingBox3D	&bbox,
		const glm::fvec3 	&planePoint,
		const glm::fvec3 	&planeNormal,
		unsigned			minId,
	       	glm::fvec3 		vertices[]
		)
{
	//Vector< float, 3 > center;
	unsigned idx = 0;
	for( unsigned i = 0; i < 12; ++i ) {
		unsigned lineAIdx = GetBBoxEdgePointA( edgeOrder[minId][i] );
		unsigned lineBIdx = GetBBoxEdgePointB( edgeOrder[minId][i] );
		if( ie_UNIQUE_INTERSECTION ==
			lineSegmentPlaneIntersection( bbox.vertices[ lineAIdx ], bbox.vertices[ lineBIdx ], planePoint, planeNormal, vertices[idx] )
		  )
		{
			/*std::cout << glm::to_string(bbox.vertices[ lineAIdx ])
				<< "; " << glm::to_string(bbox.vertices[ lineBIdx ])
				<< "; Plane Point " << glm::to_string(planePoint)
			       	<< "; " << glm::to_string(planeNormal)
				<< "\n";*/
			++idx;
			//center += vertices[idx];
			if( idx == 6 ) break;
		}
	}
	SOGLU_ASSERT( idx <= 6 ) //plane and box edges can have 6 intersections maximally
	return idx;
}

unsigned
getPlaneVerticesInBoundingBox(
		const BoundingBox3D	&bbox,
		const Planef		&plane,
		unsigned			minId,
		glm::fvec3		vertices[]
		)
{
	return getPlaneVerticesInBoundingBox(
			bbox,
			plane.point(),
			plane.normal(),
			minId,
			vertices
			);
}

unsigned
getPlaneVerticesInBoundingBox(
		const BoundingBox3D	&bbox,
		const Planef		&plane,
		glm::fvec3		vertices[]
		)
{
	unsigned maxId = 0;
	glm::fvec3 vec = glm::proj(bbox.vertices[0] - plane.point(), plane.normal() );
	float maxSize = glm::length(vec);
	int multiplier = glm::sign(glm::dot(vec, plane.normal()));
	if( multiplier == 0 ) { multiplier = 1; }
	for( unsigned i=1; i<8; ++i ) {
		vec = glm::proj(bbox.vertices[i] - plane.point(), plane.normal());
		if ( static_cast<float>( multiplier ) * glm::dot(vec, plane.normal()) > 0.0f ) {
			float tmpSize = glm::length(vec);
			if( tmpSize > maxSize ) {
				maxSize = tmpSize;
				maxId = i;
			}
		}
	}
	SOGLU_ASSERT(maxId < 8);
	return getPlaneVerticesInBoundingBox(
			bbox,
			plane.point(),
			plane.normal(),
			maxId,
			vertices
			);

}

} //namespace soglu

