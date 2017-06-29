#pragma once
#include "TextureResource.h"
#include "stb_image.h"
#include <iostream>

TextureResource::TextureResource()
{

}

TextureResource::TextureResource(const char* filename)
{
	m_fileName = filename;
}

TextureResource::TextureResource(GLenum TextureTarget, const char* filename)
{
	m_textureTarget = TextureTarget;
	m_fileName = filename;

}

bool TextureResource::Load()
{
	int width, height, numComponents;

	unsigned char *data = stbi_load(m_fileName, &width, &height, &numComponents, 4);

	if (data == NULL){

		std::cerr << "Unable to load texture: " << m_fileName << std::endl;
		return false;
	}

	glGenTextures(1, &m_texture);
	glBindTexture(m_textureTarget, m_texture);
	glTexImage2D(m_textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(m_textureTarget, 0);

	stbi_image_free(data);
	return true;
}


TextureResource::~TextureResource()
{
	if(glIsTexture(this->m_texture))
		glDeleteTextures(1, &m_texture);
}

void TextureResource::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void TextureResource::bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(m_textureTarget, m_texture);
}
