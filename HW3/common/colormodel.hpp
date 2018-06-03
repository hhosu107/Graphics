#ifndef COLORMODEL_HPP
#define COLORMODEL_HPP

#include <common/model.hpp>

using namespace std;
using namespace glm;

class ColorModel : public Model {
protected:
	// vector<vec3> m_positions from Model
	vector<vec3> m_normals;
	vector<vec3> m_colors;
	GLuint m_normal_buffer_id;
	GLuint m_color_buffer_id;
public:
	// Class constructor
	ColorModel();
	// Add vertex normal in (x,y,z) coordinate in model space
	void AddNormal(float a_nx, float a_ny, float a_nz);
	// Add vertex normal in (x,y,z) glm vector in model space
	void AddNormal(vec3 a_normal);
	// Add vertex color in (r,g,b) coordinate 
	void AddColor(float a_r, float a_g, float a_b);
	// Add vertex color in (r,g,b) glm vector 
	void AddColor(vec3 a_color);

	//void SetDepth(vector<mat4> shadowTransforms, float far_plane, vec3 lightPos);
	// Set a shader program for this model and transfer data to vertex buffers 
	void InitializeGLSL(DRAW_TYPE a_draw_type);
	// Draw function
	void Draw(vec3 lightPos);
	// Clean up all resources
	void CleanUp(void);
};

#endif
