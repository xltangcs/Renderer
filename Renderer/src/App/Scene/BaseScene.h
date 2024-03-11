#pragma once

#include "App/Scene.h"

Scene BaseScene()
{
	Scene scene("Base Scene");
	scene.objtype = MODELOBJ;

	int modelID = scene.AddModel("./assets/models/african_head.obj");

	Light redlight;
	redlight.color = glm::vec3(1.0f);
	redlight.position = glm::vec3(0.0, 0.0, 15.0);

	int redLightID = scene.AddLight(redlight);

	return scene;
}