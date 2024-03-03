#pragma once

#include <glm/glm.hpp>

#include "Core/Image.h"
#include "Core/Camera.h"
#include "App/Model.h"
#include "App/Scene.h"

enum RENDERMODE
{
	Point = 0,
	Line = 1,
	Triangle = 2
};


class Renderer
{
public:
	enum RENDERMODE rendermode;
	bool isReset = false;
public:
	Renderer();;
	~Renderer() = default;

	void OnResize(unsigned int width, unsigned int height);
	void Render(Camera& camera, Scene& scene);

	void RenderPoint(glm::vec3 color);
	void RenderLine(glm::vec3 color);

	void DrawLine(glm::vec2 p1, glm::vec2 p2, glm::vec3 color);

	std::shared_ptr<Image> GetImage() const { return  m_FinalImage; }


private:
	Camera* m_Camera = nullptr;
	Scene* m_Scene = nullptr;
	std::shared_ptr<Image> m_FinalImage = nullptr;

	uint32_t* m_ImageData = nullptr;
	bool reset = false;
};