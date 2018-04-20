#ifndef TEXTUREMODEL_HPP
#define TEXTUREMODEL_HPP

// (c) 2018 Jaehyun Jang
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <common/model.hpp>
#include <common/light.hpp>
#include <common/texture.hpp>

using namespace std;
using namespace glm;

class TextureModel : public Model {
protected:
	// vector<vec3> m_positions from Model
	vector<vec3> m_normals;
	vector<vec2> m_uvs;

	GLuint m_normal_buffer_id;
	GLuint m_uv_buffer_id;
	
    Texture* m_color_texture;
public:
	// Class constructor
	TextureModel();
	// Add vertex normal in (x,y,z) coordinate in model space
	void AddNormal(float a_nx, float a_ny, float a_nz);
	// Add vertex normal in (x,y,z) glm vector in model space
	void AddNormal(vec3 a_normal);
	// Add uv coordinate to this model
	void AddUV(float a_u, float a_v);
	// Add uv vector to this model
	void AddUV(vec2 a_uv);
	// Bind texture
	void SetColorTexture(Texture* a_texture);
    // Set a shader program for this model and transfer data to vertex buffers 
	void InitializeGLSL(DRAW_TYPE a_draw_type);
	// Draw function
	void Draw(void);
	// Clean up all resources
	void CleanUp(void);
};

#endif
