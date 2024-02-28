#include "Image.h"

#include <iostream>
#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image(uint32_t width, uint32_t height, GLenum type, const void* data)
    : m_Width(width), m_Height(height), m_DataFormat(type)
{
    CreatImage();
    if (data)
        SetData(data);
}


Image::Image(const std::string& path)
{
    glGenTextures(1, &m_TextureID);

    int width, height, nrComponents;
    uint8_t* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    GLenum format = 0;
    if (data)
    {
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        
        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        m_Width = width;
        m_Height = height;
        m_DataFormat = format;

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
}

Image::~Image()
{
	glDeleteTextures(1, &m_TextureID);
}

void Image::SetData(const void* data)
{
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_Data = (uint8_t*)data;
}

void Image::Resize(uint32_t width, uint32_t height)
{
    if (m_Width == width && m_Height == height)
        return;
    m_Width = width;
    m_Height = height;
    CreatImage();
}

void Image::CreatImage()
{
    //glGenTextures(1, &m_TextureID);
    //glBindTexture(GL_TEXTURE_2D, m_TextureID);    
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glBindTexture(GL_TEXTURE_2D, 0);


    // create a color attachment texture
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

