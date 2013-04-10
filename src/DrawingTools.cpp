#include "soglu/DrawingTools.hpp"

#ifdef DIS_0
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
	ASSERT( idx < 12 ); //only 12 edges

	/*if( idx < 8 ) {
		return idx % 4;
	}
	return idx - 4;*/
	return edgeVertexAMapping[idx]; 
}

static unsigned
GetBBoxEdgePointB( unsigned idx )
{
	ASSERT( idx < 12 ); //only 12 edges

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
		       	unsigned		&minId,	
		       	unsigned		&maxId	
			)
{
	min = glm::length( vectorProjection( direction, bbox.vertices[0] - eyePoint ) );
	minId = 0;
	max = glm::length( vectorProjection( direction, bbox.vertices[0] - eyePoint ) );
	maxId = 0;
	for( unsigned i=1; i<8; ++i ) {
		float tmpSize = glm::length( vectorProjection( direction, bbox.vertices[i] - eyePoint ) );
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

} //namespace soglu
#endif
