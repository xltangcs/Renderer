#pragma once

#include <memory>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Geometry/BufferGeometry.h"
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

};

enum OBJECTTYPE
{
	MODELOBJ = 0,
	GEOMETRYOBJ = 1
};

class Scene
{
public:
	char* sceneName;
	enum OBJECTTYPE objtype;
	glm::vec3 baseColor = glm::vec3(1.0);
	std::vector<std::shared_ptr<Model>> m_Models;
	std::vector<Light> m_Lights;
	std::vector<Triangle> m_Triangles;
public:
	Scene(char* name = "Scene");
	~Scene() = default;

	int AddModel(const std::string& filename);
	int AddGeometry(const BufferGeometry& geo);
	int AddLight(Light light);

	void ProcessScene();
private:
	//std::vector<std::unique_ptr<Model>> m_Models;
	//std::vector<Light> m_Lights;
};