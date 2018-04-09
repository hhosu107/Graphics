// This code is licensed under MIT license (see LICENSE.txt for details)

#include <iostream>
#include <vector>
#include <sstream>

#include "rtt.hpp"
using namespace std;
using namespace glm;

RTT::RTT()
{
	m_texture = Texture();
	m_fbo = 0;
}

RTT::~RTT()
{
	if (m_fbo != 0)
	{
		glDeleteFramebuffers(1, &m_fbo);
	}
}

void RTT::CreateTexture(double a_width, double a_height)
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, a_width, a_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER 
		GL_COLOR_ATTACHMENT0,  // 2. attachment point
		GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
		texture_id,             // 4. tex ID
		0);                    // 5. mipmap level: 0(base)
	m_texture.SetTexture(texture_id);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RTT::BindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void RTT::UnbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Texture* RTT::GetTexture()
{
	return &m_texture;
}
