#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "SOIL/SOIL.h"

using namespace std;
using namespace glm;

class Texture {

protected:
	GLuint m_texture_unit;
public:
	Texture();
	~Texture();
	GLuint GetTexture();
	Texture* GetTex();
	void SetTexture(GLuint a_texture);
	void SetTexture(const char* name);
};

#endif
