#pragma once

namespace soglu
{

struct ViewConfiguration2D
{
	ViewConfiguration2D(	
			glm::fvec2	aCenterPoint = glm::fvec2(),
			float			aZoom = 1.0f,
			bool			aHFlip = false,
			bool			aVFlip = false
			): centerPoint( aCenterPoint ), zoom( aZoom ), hFlip( aHFlip ), vFlip( aVFlip )
	{
	}

	glm::fvec2	centerPoint;
	float			zoom;
	bool			hFlip;
	bool			vFlip;
};

/*
struct SliceViewConfig
{
	SliceViewConfig(): plane( XY_PLANE ), currentSlice( 0 )
	{}

	CartesianPlanes		plane;

	Vector< int32, 3 >	currentSlice;

	ViewConfiguration2D	viewConfiguration;
};



ViewConfiguration2D 
GetOptimalViewConfiguration( const Vector2f &regionSize, const Vector< uint32, 2 > &windowSize, ZoomType zoomType = ztFIT );

ViewConfiguration2D 
GetOptimalViewConfiguration( const Vector2f &regionMin, const Vector2f &regionMax, const Vector< uint32, 2 > &windowSize, ZoomType zoomType = ztFIT );

Vector2f
GetRealCoordinatesFromScreen( const Vector2f &aScreenPos, const Vector< uint32, 2 > &windowSize, const ViewConfiguration2D &aConfig );
*/
}/*namespace soglu*/

