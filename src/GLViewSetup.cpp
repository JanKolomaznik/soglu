#include <soglu/GLViewSetup.hpp>
#include <soglu/Camera.hpp>


namespace soglu {

void
getProjectionAndViewMatricesFromOrthoCamera( const OrthoCamera &camera, glm::dmat4x4 &aProjection, glm::dmat4x4 &aView )
{
	aProjection = glm::ortho(
				camera.topLeftCorner().x,
				camera.bottomRightCorner().x,
				camera.bottomRightCorner().y,
				camera.topLeftCorner().y,
				camera.near(),
				camera.far()
				);
	/*std::cout << "topLeftCorner : " << glm::to_string(camera.topLeftCorner()) << "\n";
	std::cout << "bottomRightCorner : " << glm::to_string(camera.bottomRightCorner()) << "\n";
	std::cout << "near : " << camera.near() << "\n";
	std::cout << "far : " << camera.far() << "\n";*/
	aView = glm::lookAt<double>(
		glm::dvec3(camera.eyePosition()),
		glm::dvec3(camera.targetPosition()),
		glm::dvec3(camera.upDirection())
		);
}

void
getProjectionAndViewMatricesFromCamera( const Camera &camera, glm::dmat4x4 &aProjection, glm::dmat4x4 &aView )
{
	aProjection = glm::perspective<double>(
		camera.fieldOfView(),
		camera.aspectRatio(),
		camera.near(),
		camera.far()
	);

	aView = glm::lookAt<double>(
		glm::dvec3(camera.eyePosition()),
		glm::dvec3(camera.targetPosition()),
		glm::dvec3(camera.upDirection())
	);
}

GLViewSetup
getViewSetupFromCamera( const Camera &camera )
{
	GLViewSetup setup;

	getProjectionAndViewMatricesFromCamera( camera, setup.projection, setup.view );
	setup.modelView = setup.view;
	setup.modelViewProj = setup.projection * setup.modelView;
	return setup;
}

GLViewSetup
getViewSetupFromOrthoCamera( const OrthoCamera &camera )
{
	GLViewSetup setup;

	getProjectionAndViewMatricesFromOrthoCamera( camera, setup.projection, setup.view );
	setup.modelView = setup.view;
	setup.modelViewProj = setup.projection * setup.modelView;
	return setup;
}

GLViewSetup
getViewSetupForViewportOrthoView(
		const glm::ivec2 &aTopLeftCorner,
		const glm::ivec2 &aBottomRightCorner,
		float aNear,
		float aFar
		)
{
	GLViewSetup setup;
	setup.projection = glm::ortho<double>(
				aTopLeftCorner.x,
				aBottomRightCorner.x,
				aBottomRightCorner.y,
				aTopLeftCorner.y,
				aNear,
				aFar
				);
	setup.viewport = glm::ivec4(aTopLeftCorner, aBottomRightCorner);
	setup.view = glm::dmat4x4(1.0);

/*	glm::dvec3 center = 0.5 * glm::dvec3(aTopLeftCorner + aBottomRightCorner, 0.0);
	glm::dvec3 eye = center;
	center[2] = 100.0;

	setup.view = glm::lookAt<double>(
		eye,
		center,
		glm::dvec3(0, 1, 0)
		);*/

	setup.modelView = setup.view;
	setup.modelViewProj = setup.projection * setup.modelView;
	return setup;
}




} //namespace soglu
