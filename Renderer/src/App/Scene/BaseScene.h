#pragma once

#include "App/Scene.h"

Scene BaseScene()
{
	Scene scene("Base Scene");
	scene.objtype = MODELOBJ;

	int modelID = scene.AddModel("./assets/models/african_head.obj");

	Light redlight;
	redlight.color = glm::vec3(300.0f, 300.0f, 300.0f);
	redlight.position = glm::vec3(0.0, 0.0, 5.0);

	int redLightID = scene.AddLight(redlight);

	return scene;
}