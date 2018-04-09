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
	if(m_texture_unit != 0)
	{
		glDeleteTextures(1, &m_texture_unit);
	}
}

GLuint Texture::GetTexture()
{
	return m_texture_unit;
}

void Texture::SetTexture(GLuint a_texture)
{
	m_texture_unit = a_texture;
}
