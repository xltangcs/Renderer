#pragma once

#include <glm/glm.hpp>

#include "Core/Image.h"
#include "Core/Camera.h"
#include "App/Model.h"
#include "App/Scene.h"
#include "App/Triangle.h"

enum RENDERMODE
{
	Point_MODE = 0,
	Line_MODE = 1,
	Triangle_MODE = 2
};

enum LIGHTMODELMODE
{
	Lambert = 0,
	Phong = 1,
	Blin_Phong = 2,
	PBR = 3
};



class Renderer
{
public:
	enum RENDERMODE rendermode;
	enum LIGHTMODELMODE lightmodelmode;
	//bool isRotate = false;
	bool isReset = false;


	glm::mat4 modelMatrix = glm::mat4(1.0f);

	
public:
	Renderer();;
	~Renderer() = default;

	void OnResize(unsigned int width, unsigned int height);
	void Render(Camera& camera, Scene& scene);

	std::shared_ptr<Image> GetImage() const { return  m_FinalImage; }
private:

	glm::vec3 CalculateLambertColor(glm::vec3 worldPos, glm::vec3 normal, Light light);
	glm::vec3 CalculatePhongColor(glm::vec3 worldPos, glm::vec3 normal, Light light);
	glm::vec3 CalculateBlin_PhongColor(glm::vec3 worldPos, glm::vec3 normal, Light light);
	glm::vec3 CalculatePBRColor(glm::vec3 baseColor, glm::vec3 worldPos, glm::vec3 normal, Light light);

	void ResetData();

	bool InsideTriangle(glm::vec2 p, std::vector<glm::vec3> t);
	std::tuple<float, float, float> computeBarycentric2D(glm::vec2 p, std::vector<glm::vec3> t);

	void RasterizePoint(glm::vec3 color);
	void RasterizeLine(glm::vec3 color);
	void RasterizeTriangle(glm::vec3 color);

	void DrawPoint(glm::vec2 p, glm::vec3 color);
	void DrawLine(glm::vec2 p0, glm::vec2 p1, glm::vec3 color);
	void Renderer::DrawTriangle(Triangle t, std::vector<glm::vec3> viewPos, std::vector<float> w, glm::vec3 color);

private:
	unsigned int frameIndex = 0;
	uint32_t m_Width = 10, m_Height = 10;
	Camera* m_Camera = nullptr;
	Scene* m_Scene = nullptr;
	std::shared_ptr<Image> m_FinalImage = nullptr;

	uint32_t* m_ImageData = nullptr;
	std::vector<float> z_buffer;
	//uint8_t* z_buffer = nullptr;
	bool reset = false;



};