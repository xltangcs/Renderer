#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Core/Random.h"

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
	rendermode = Triangle_MODE;
	lightmodelmode = Lambert;

	m_FinalImage = std::make_shared<Image>(m_Width, m_Height);
	z_buffer.resize(m_Width * m_Height);
	
}

void Renderer::OnResize(unsigned int width, unsigned int height)
{
	if (m_Width == width && m_Height == height) return;

	m_Width = width; m_Height = height;
	m_FinalImage->Resize(m_Width, m_Height);

	delete[] m_ImageData;
	m_ImageData = new uint32_t[m_Width * m_Height];
	memset(m_ImageData, 0, m_Width * m_Height * sizeof(uint32_t));

	z_buffer.resize(m_Width * m_Height);
}


void Renderer::Render(Camera& camera, Scene& scene)
{
	std::fill(z_buffer.begin(), z_buffer.end(), std::numeric_limits<float>::infinity());
	frameIndex ++ ;
	//memset(m_ImageData, 0, m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(uint32_t));

	if (camera.isCameraMoved || isReset)
	{
		memset(m_ImageData, 0, m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(uint32_t));
		isReset = false;
		frameIndex = 0;
	}

	m_Camera = &camera;
	m_Scene = &scene;

	glm::vec3 color = glm::vec3(1.0f);
	switch (rendermode)
	{
	case Point_MODE: RasterizePoint(color);
		break;
	case Line_MODE: RasterizeLine(color);
		break;
	case Triangle_MODE: RasterizeTriangle(color);
		break;
	default:
		break;
	}

	m_FinalImage->SetData(m_ImageData);
}


glm::vec3 Renderer::CalculateLambertColor(float kd, glm::vec3 viewPos, glm::vec3 normal, Light light)
{
	normal = glm::normalize(normal);
	glm::vec3 dir = glm::normalize(light.position - viewPos);
	glm::vec3 color = light.color* kd * std::max(glm::dot(normal, dir), 0.0f);

	return color;
}

glm::vec3 Renderer::CalculatePhongtColor(float kd, glm::vec3 normal, Light light)
{
	return glm::vec3();
}

void Renderer::ResetData()
{
	delete[] m_ImageData;
	m_ImageData = new uint32_t[m_Width * m_Height];
	memset(m_ImageData, 0, m_Width * m_Height * sizeof(uint32_t));

	z_buffer.resize(m_Width * m_Height);
}

bool Renderer::InsideTriangle(glm::vec2 p, std::vector<glm::vec3> t)
{
	for (int i = 0; i < 3; i++)
		t[i].z = 0.0f;
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
	//float x = p.x, y = p.y;
	//float c1 = (x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * y + v[1].x * v[2].y - v[2].x * v[1].y) / (v[0].x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * v[0].y + v[1].x * v[2].y - v[2].x * v[1].y);
	//float c2 = (x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * y + v[2].x * v[0].y - v[0].x * v[2].y) / (v[1].x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * v[1].y + v[2].x * v[0].y - v[0].x * v[2].y);
	//float c3 = (x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * y + v[0].x * v[1].y - v[1].x * v[0].y) / (v[2].x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * v[2].y + v[0].x * v[1].y - v[1].x * v[0].y);
	//return { c1,c2,c3 };

	glm::vec3 A = glm::vec3(v[0].x, v[0].y, 0.0f);
	glm::vec3 B = glm::vec3(v[1].x, v[1].y, 0.0f);
	glm::vec3 C = glm::vec3(v[2].x, v[2].y, 0.0f);
	glm::vec3 P = glm::vec3(p, 0.0f);
	// 计算三角形的面积
	float areaABC = glm::length(glm::cross(B - A, C - A));
	// 计算 p 相对于 ABC 三角形的重心坐标
	float alpha = glm::length(glm::cross(B - P, C - P)) / areaABC;
	float beta = glm::length(glm::cross(C - P, A - P)) / areaABC;
	float gamma = 1.0f - alpha - beta;
	return std::make_tuple(alpha, beta, gamma);
}


void Renderer::RasterizePoint(glm::vec3 color)
{
	for(auto triangle : m_Scene->m_Triangles)
	{
		//std::vector<glm::vec3> facei = model->GetFace(i);
		for (int j = 0; j < 3; j++)
		{
			glm::vec3 v0 = triangle.vert[j];
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
	for (auto triangle : m_Scene->m_Triangles)
	{
		for (int j = 0; j < 3; j++)
		{
			glm::vec3 v0 = triangle.vert[j];
			glm::vec3 v1 = triangle.vert[(j + 1) % 3];

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
	for (auto triangle : m_Scene->m_Triangles)
	{
		Triangle t;
		std::vector<float> w(3);
		std::vector<glm::vec3> viewPos;

		for (int i = 0; i < 3; i++)
		{
			glm::vec3 modelPos = triangle.vert[i];

			viewPos.push_back(glm::vec3(m_Camera->GetView() * modelMatrix * glm::vec4(modelPos, 1.0f)));

			glm::vec4 clipPos = m_Camera->GetProjection() * m_Camera->GetView() * modelMatrix * glm::vec4(modelPos, 1.0f);

			glm::vec3 screenCoords;
			
			screenCoords.x = clipPos.x / clipPos.w * m_FinalImage->GetWidth() / 2.0 + m_FinalImage->GetWidth() / 2.0;
			screenCoords.y = -clipPos.y / clipPos.w * m_FinalImage->GetHeight() / 2.0 + m_FinalImage->GetHeight() / 2.0;
			screenCoords.z = clipPos.z / clipPos.w;;

			w[i] = clipPos.w;

			t.vert.push_back(screenCoords);

			t.normal.push_back(glm::transpose(m_Camera->GetInverseView())* glm::transpose(glm::inverse(modelMatrix))* glm::vec4(triangle.normal[i], 0.0f));
			
			t.uv.push_back(triangle.uv[i]);

		}
		
		DrawTriangle(t, viewPos, w, color);
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

void Renderer::DrawTriangle(Triangle t, std::vector<glm::vec3> viewPos, std::vector<float> w, glm::vec3 color)
{
	//First: Ceater bounding box
	float minX = std::max(std::min(t.vert[2].x, std::min(t.vert[0].x, t.vert[1].x)), 0.0f);
	float maxX = std::min(std::max(t.vert[2].x, std::max(t.vert[0].x, t.vert[1].x)), (float)m_Width);
	float minY = std::max(std::min(t.vert[2].y, std::min(t.vert[0].y, t.vert[1].y)), 0.0f);
	float maxY = std::min(std::max(t.vert[2].y, std::max(t.vert[0].y, t.vert[1].y)), (float)m_Height);

	minX = (int)std::floor(minX); 
	maxX = (int)std::ceil(maxX); 
	minY = (int)std::floor(minY); 
	maxY = (int)std::ceil(maxY);

	for (int i = minX; i < maxX; i++)
	{
		for (int j = minY; j < maxY; j++)
		{
			int Index = i + j * m_Width;

			if(!InsideTriangle(glm::vec2(i + 0.5f, j + 0.5f), t.vert)) continue;

			auto [alpha, beta, gamma] = computeBarycentric2D(glm::vec2(i + 0.5f, j + 0.5f), t.vert);

			if (alpha < 0 || beta < 0 || gamma < 0) continue;

			float Z = 1.0 / (alpha / w[0] + beta / w[1] + gamma / w[2]);

			float z_interpolated = (alpha * t.vert[0].z / w[0] + beta * t.vert[1].z / w[1] + gamma * t.vert[2].z / w[2]) * Z; 
			glm::vec3 normal_interpolated = (alpha * t.normal[0] / w[0] + beta * t.normal[1] / w[1] + gamma * t.normal[2] / w[2]) * Z;
			glm::vec3 viewPos_interpolated = (alpha * viewPos[0] / w[0] + beta * viewPos[1] / w[1] + gamma * viewPos[2] / w[2]) * Z;
			
			normal_interpolated = glm::normalize(normal_interpolated);

			glm::vec2 uv_interpolated = (alpha * t.uv[0] / w[0] + beta * t.uv[1] / w[1] + gamma * t.uv[2] / w[2]) * Z;
			
			// check zbuff
			if (z_interpolated < z_buffer[Index])
			{
				z_buffer[Index] = z_interpolated;
				auto light = m_Scene->m_Lights[0];

				glm::vec3 color = glm::vec3(0.0);
				glm::vec3 baseColor = glm::vec3(1.0);
				switch (m_Scene->objtype)
				{
				case MODELOBJ : baseColor = m_Scene->m_Models[0]->diffusImage->GetPixel(uv_interpolated);
					break;
				case GEOMETRYOBJ: baseColor = m_Scene->baseColor;
				default:
					break;
				}

				switch (lightmodelmode)
				{
				case Lambert: color = baseColor * CalculateLambertColor(1.0, viewPos_interpolated, normal_interpolated, light);
					break;
				case Phong: color = baseColor * CalculatePhongtColor(1.0, normal_interpolated, light);
					break;
				case Blin_Phong:
					break;
				case PBR:
					break;
				default:
					break;
				}




				DrawPoint(glm::vec2(i, j), color);
			}
			
		}
	}
}

