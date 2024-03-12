#pragma once

#include "App/Scene.h"
#include "Core/Geometry/SphereGeometry.h"

Scene PBRScene()
{
	Scene scene("PBR Scene");
	scene.objtype = GEOMETRYOBJ;

	SphereGeometry geo(0.5, 32, 32);

	scene.AddGeometry(geo);
	scene.baseColor = glm::vec3(0.5, 0.0, 0.0);

	Light redlight;
	redlight.color = glm::vec3(300.0f, 300.0f, 300.0f);
	redlight.position = glm::vec3(0.0, 0.0, 5.0);

	int redLightID = scene.AddLight(redlight);

	return scene;
}