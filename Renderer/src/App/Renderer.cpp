#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Utils
{
	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
}

Renderer::Renderer()
{
	rendermode = Point;
}

void Renderer::OnResize(unsigned int width, unsigned int height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;
		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Image>(width, height);
	}
	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
	memset(m_ImageData, 0, width * height * sizeof(uint32_t));
}


void Renderer::Render(Camera& camera, Scene& scene)
{
	if (camera.isCameraMoved || isReset)
	{
		memset(m_ImageData, 0, m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(uint32_t));
		isReset = false;
	}

	m_Camera = &camera;
	m_Scene = &scene;

	glm::vec3 color = glm::vec3(1.0f);
	switch (rendermode)
	{
	case Point: RenderPoint(color);
		break;
	case Line: RenderLine(color);
		break;
	case Triangle:
		break;
	default:
		break;
	}

	m_FinalImage->SetData(m_ImageData);
}

void Renderer::RenderPoint(glm::vec3 color)
{
	auto model = m_Scene->m_Models[0];

	for (int i = 0; i < model->nFaces(); i++)
	{
		std::vector<int> facei = model->GetFace(i);
		for (int j = 0; j < 3; j++)
		{
			glm::vec3 v0 = model->GetVert(facei[j]);

			glm::vec4 vv0 = m_Camera->GetProjection() * m_Camera->GetView() * glm::vec4(v0, 1.0f);

			int x0 = vv0.x / vv0.w * m_FinalImage->GetWidth() / 2.0 + m_FinalImage->GetWidth() / 2.0;
			int y0 = -vv0.y / vv0.w * m_FinalImage->GetHeight() / 2.0 + m_FinalImage->GetHeight() / 2.0;

			if (x0 >= 0 && x0 < m_FinalImage->GetWidth() && y0 >= 0 && y0 < m_FinalImage->GetHeight())
				m_ImageData[x0 + y0 * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(glm::vec4(color, 1.0f));
		}
	}
}

void Renderer::RenderLine(glm::vec3 color)
{
	auto model = m_Scene->m_Models[0];

	for (int i = 0; i < model->nFaces(); i++)
	{
		std::vector<int> facei = model->GetFace(i);
		for (int j = 0; j < 3; j++)
		{
			glm::vec3 v0 = model->GetVert(facei[j]);
			glm::vec3 v1 = model->GetVert(facei[(j + 1) % 3]);

			glm::vec4 vv0 = m_Camera->GetProjection() * m_Camera->GetView() * glm::vec4(v0, 1.0f);
			glm::vec4 vv1 = m_Camera->GetProjection() * m_Camera->GetView() * glm::vec4(v1, 1.0f);

			int x0 = vv0.x / vv0.w * m_FinalImage->GetWidth() / 2.0 + m_FinalImage->GetWidth() / 2.0;
			int y0 = -vv0.y / vv0.w * m_FinalImage->GetHeight() / 2.0 + m_FinalImage->GetHeight() / 2.0;

			int x1 = vv1.x / vv1.w * m_FinalImage->GetWidth() / 2.0 + m_FinalImage->GetWidth() / 2.0;
			int y1 = -vv1.y / vv1.w * m_FinalImage->GetHeight() / 2.0 + m_FinalImage->GetHeight() / 2.0;

			if (x0 >= 0 && x0 < m_FinalImage->GetWidth() && x1 >= 0 && x1 < m_FinalImage->GetWidth()
				&& y0 >= 0 && y0 < m_FinalImage->GetHeight() && y1 >= 0 && y1 < m_FinalImage->GetHeight())
				DrawLine(glm::vec2(x0, y0), glm::vec2(x1, y1), color);
		}
	}
}

void Renderer::DrawLine(glm::vec2 p1, glm::vec2 p2, glm::vec3 color)
{
	int x0 = p1.x, y0 = p1.y;
	int x1 = p2.x, y1 = p2.y;

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (x0 != x1 || y0 != y1) 
	{
		m_ImageData[x0 + y0 * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(glm::vec4(color, 1.0f));
		int err2 = 2 * err;
		if (err2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (err2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
	m_ImageData[x1 + y1 * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(glm::vec4(color, 1.0f));
}

