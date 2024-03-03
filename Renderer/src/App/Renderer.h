#pragma once

#include <glm/glm.hpp>

#include "Core/Image.h"
#include "Core/Camera.h"
#include "App/Model.h"
#include "App/Scene.h"

class Renderer
{
public:
	Renderer() = default;;
	~Renderer() = default;

	void OnResize(unsigned int width, unsigned int height);
	void Render(Camera& camera, Scene& scene);
	

	void Line(glm::vec2 p1, glm::vec2 p2);

	std::shared_ptr<Image> GetImage() const { return  m_FinalImage; }


private:
	std::shared_ptr<Image> m_FinalImage = nullptr;
	uint32_t* m_ImageData = nullptr;
};