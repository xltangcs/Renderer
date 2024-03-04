#pragma once

#include <memory>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Image.h"
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

struct Light
{
	glm::vec3 color;

	glm::vec3 position;
	glm::vec3 dir;

};

class Scene
{
public:
	char* sceneName;
	std::vector<std::shared_ptr<Model>> m_Models;
	std::vector<Light> m_Lights;
	std::shared_ptr<Image> texture;
public:
	Scene(char* name = "Scene");
	~Scene() = default;

	int AddModel(const std::string& filename);
	int AddLight(Light light);

	void ProcessScene();
private:
	//std::vector<std::unique_ptr<Model>> m_Models;
	//std::vector<Light> m_Lights;
};