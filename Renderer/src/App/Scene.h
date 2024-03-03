#pragma once

#include <memory>
#include <glm/gtc/matrix_transform.hpp>

#include "App/Model.h"
#include "App/Triangle.h"

struct ModelInstance
{
	glm::mat4 transform;
	std::string name;

	int modelID;

	ModelInstance(std::string name, int modelId, glm::mat4 xform)
		: name(name), modelID(modelId), transform(xform)
	{
	}
	~ModelInstance() {}
};

class Scene
{
public:
	char* sceneName;
	std::vector<Triangle> m_Triangles;
	std::vector<std::shared_ptr<Model>> m_Models;
public:
	Scene(char* name = "Scene") : sceneName(name) { };
	~Scene() = default;

	int AddModel(const std::string& filename);

	void ProcessScene();
private:
	//std::vector<std::unique_ptr<Model>> m_Models;
	//std::vector<Light> m_Lights;
};