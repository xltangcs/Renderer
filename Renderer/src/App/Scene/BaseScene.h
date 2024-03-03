#pragma once

#include "App/Scene.h"

Scene CreatBaseScene()
{
	Scene scene("Base Scene");

	int modelID = scene.AddModel("./assets/models/african_head.obj");

	return scene;
}