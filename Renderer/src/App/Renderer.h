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

	std::shared_ptr<Image> GetImage() const { return  m_FinalImage; }
private:

	void ResetData();

	bool InsideTriangle(glm::vec2 p, std::vector<glm::vec3> t);
	std::tuple<float, float, float> computeBarycentric2D(glm::vec2 p, std::vector<glm::vec3> t);


	void RasterizePoint(glm::vec3 color);
	void RasterizeLine(glm::vec3 color);
	void RasterizeTriangle(glm::vec3 color);

	void DrawPoint(glm::vec2 p, glm::vec3 color);
	void DrawLine(glm::vec2 p0, glm::vec2 p1, glm::vec3 color);
	void DrawTriangle(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec3 color);
	void DrawTriangle(std::vector<glm::vec3> t, glm::vec3 color);

private:
	uint32_t m_Width = 10, m_Height = 10;
	Camera* m_Camera = nullptr;
	Scene* m_Scene = nullptr;
	std::shared_ptr<Image> m_FinalImage = nullptr;

	uint32_t* m_ImageData = nullptr;
	uint8_t* z_buffer = nullptr;
	bool reset = false;
};