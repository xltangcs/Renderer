#include "Renderer.h"

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
}



void Renderer::Render(Camera& camera, Scene& scene)
{
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec3 color = glm::vec3(0.6f, 0.1f, 0.1f);

			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(glm::vec4(color, 1.0f));
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

void Renderer::Line(glm::vec2 p1, glm::vec2 p2)
{


}

