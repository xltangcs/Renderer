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
	rendermode = Triangle;

	m_FinalImage = std::make_shared<Image>(m_Width, m_Height);
}

void Renderer::OnResize(unsigned int width, unsigned int height)
{
	if (m_Width == width && m_Height == height) return;

	m_Width = width; m_Height = height;
	m_FinalImage->Resize(m_Width, m_Height);

	ResetData();
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
	case Point: RasterizePoint(color);
		break;
	case Line: RasterizeLine(color);
		break;
	case Triangle: RasterizeTriangle(color);
		break;
	default:
		break;
	}

	m_FinalImage->SetData(m_ImageData);
}

void Renderer::ResetData()
{
	delete[] m_ImageData;
	delete[] z_buffer;
	m_ImageData = new uint32_t[m_Width * m_Height];
	z_buffer = new uint8_t[m_Width * m_Height];
	memset(m_ImageData, 0, m_Width * m_Height * sizeof(uint32_t));
	memset(z_buffer, 0, m_Width * m_Height * sizeof(uint8_t));
}

bool Renderer::InsideTriangle(glm::vec2 p, std::vector<glm::vec3> t)
{
	glm::vec3 P = glm::vec3(p, 0.0f);
	glm::vec3 AC = t[2] - t[0];
	glm::vec3 CB = t[1] - t[2];
	glm::vec3 BA = t[0] - t[1];
	glm::vec3 AP = P - t[0];
	glm::vec3 BP = P - t[1];
	glm::vec3 CP = P - t[2];

	//if cross product in the same direction ,its inside the triangle
	if (glm::dot(glm::cross(AP, AC), glm::cross(BP, BA)) > 0.0f &&
		glm::dot(glm::cross(BP, BA), glm::cross(CP, CB)) > 0.0f &&
		glm::dot(glm::cross(CP, CB), glm::cross(AP, AC)) > 0.0f)
	{
		return true;
	}

	return false;
}

std::tuple<float, float, float> Renderer::computeBarycentric2D(glm::vec2 p, std::vector<glm::vec3> v)
{
	float c1 = (p.x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * p.y + v[1].x * v[2].y - v[2].x * v[1].y) / (v[0].x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * v[0].y + v[1].x * v[2].y - v[2].x * v[1].y);
	float c2 = (p.x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * p.y + v[2].x * v[0].y - v[0].x * v[2].y) / (v[1].x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * v[1].y + v[2].x * v[0].y - v[0].x * v[2].y);
	float c3 = (p.x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * p.y + v[0].x * v[1].y - v[1].x * v[0].y) / (v[2].x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * v[2].y + v[0].x * v[1].y - v[1].x * v[0].y);
	return { c1,c2,c3 };
}

void Renderer::RasterizePoint(glm::vec3 color)
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
				DrawPoint(glm::vec2(x0, y0), color);
		}
	}
}

void Renderer::RasterizeLine(glm::vec3 color)
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

void Renderer::RasterizeTriangle(glm::vec3 color)
{
	auto model = m_Scene->m_Models[0];
	for (int i = 0; i < model->nFaces(); i++)
	{
		std::vector<int> facei = model->GetFace(i);
		std::vector<glm::vec3> screenCoords(3);
		std::vector<glm::vec3> modelCoords(3);
		for (int i = 0; i < 3; i++)
		{
			modelCoords[i] = model->GetVert(facei[i]);
			glm::vec4 clipCoords = m_Camera->GetProjection() * m_Camera->GetView() * glm::vec4(modelCoords[i], 1.0f);

			screenCoords[i].x = clipCoords.x / clipCoords.w * m_FinalImage->GetWidth() / 2.0 + m_FinalImage->GetWidth() / 2.0;
			screenCoords[i].y = -clipCoords.y / clipCoords.w * m_FinalImage->GetHeight() / 2.0 + m_FinalImage->GetHeight() / 2.0;
			screenCoords[i].z = clipCoords.z / clipCoords.w;
		}

		DrawTriangle(screenCoords, color);
	}
}

void Renderer::DrawPoint(glm::vec2 p, glm::vec3 color)
{
	int index = p.x + p.y * m_FinalImage->GetWidth();
	m_ImageData[index] = Utils::ConvertToRGBA(glm::vec4(color, 1.0f));;
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
		DrawPoint(glm::vec2(x0, y0), color);
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
	DrawPoint(glm::vec2(x1, y1), color);
}

void Renderer::DrawTriangle(std::vector<glm::vec3> t, glm::vec3 color)
{
	bool IsUseSurperSampling = true;

	//Sample Offset
	std::vector<glm::vec3> SampleOffset = {
		{0.25f,0.25f,0},
		{0.25f,0.75f,0},
		{0.75f,0.25f,0},
		{0.75f,0.75f,0}
	};

	//First: Ceater bounding box
	float minX = std::max(std::min(t[2].x, std::min(t[0].x, t[1].x)), 0.0f);
	float maxX = std::min(std::max(t[2].x, std::max(t[0].x, t[1].x)), (float)m_Width);

	float minY = std::max(std::min(t[2].y, std::min(t[0].y, t[1].y)), 0.0f);
	float maxY = std::min(std::max(t[2].y, std::max(t[0].y, t[1].y)), (float)m_Height);

	minX = (int)std::floor(minX); 
	maxX = (int)std::ceil(maxX); 
	minY = (int)std::floor(minY); 
	maxY = (int)std::ceil(maxY);

	for (int i = minX; i < maxX; i++)
	{
		for (int j = minY; j < maxY; j++)
		{
			int Index = i + j * m_FinalImage->GetWidth();
			if (InsideTriangle(glm::vec2(i + 0.5f, j + 0.5f), t))
			{

				auto [alpha, beta, gamma] = computeBarycentric2D(glm::vec2(i + 0.5f, j + 0.5f), t);
				float z_interpolated = alpha * t[0].z + beta * t[1].z + gamma * t[2].z;

				// check zbuff
				if (z_interpolated > z_buffer[Index])
				{
					z_buffer[Index] = z_interpolated;
					DrawPoint(glm::vec2(i, j), color);
				}
			}
		}
	}
}

