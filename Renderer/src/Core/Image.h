#pragma once

#include <string>

#include <glad/glad.h>

#include <glm/glm.hpp>

class Image
{
public:
	Image(uint32_t width, uint32_t height, GLenum type = GL_RGBA, const void* data = nullptr);
	Image(const std::string& path);
	~Image();

	void SetData(const void* data);
	void Resize(uint32_t width, uint32_t height);

	void SetPixel(int x, int y, glm::vec3 color);

	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
	uint32_t GetTextureID() const { return m_TextureID; }
private:
	void CreatImage();

private:
	uint32_t m_Width = 0, m_Height = 0;
	uint32_t m_TextureID = 0;
	uint8_t* m_Data = nullptr;
	GLenum m_DataFormat = 0;
};
