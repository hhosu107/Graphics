// This code is licensed under MIT license (see LICENSE.txt for details)

#include <iostream>
#include <vector>
#include <sstream>

#include "texture.hpp"
using namespace std;
using namespace glm;

Texture::Texture()
{
	m_texture_unit = 0;
}

Texture::~Texture()
{
	if (m_texture_unit != 0)
	{
		glDeleteTextures(1, &m_texture_unit);
	}
}

GLuint Texture::GetTexture()
{
	return m_texture_unit;
}

// If the instance is attached to RTT instance, this function will not be used.
void Texture::SetTexture(const char* name)
{
	int a_width, a_height;
	GLuint texture_id;
	unsigned char* image = SOIL_load_image(name, &a_width, &a_height, 0, SOIL_LOAD_RGB);
	if (image == NULL) {
		printf("Soil loading error: '%s'\n", SOIL_last_result());
	}

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, a_width, a_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	/*
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
	GL_COLOR_ATTACHMENT0,  // 2. attachment point
	GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
	texture_id,             // 4. tex ID
	0);                    // 5. mipmap level: 0(base)
	*/
	SetTexture(texture_id);
	SOIL_free_image_data(image);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Texture* Texture::GetTex()
{
	return this;
}


void Texture::SetTexture(GLuint a_texture)
{
	m_texture_unit = a_texture;
}
