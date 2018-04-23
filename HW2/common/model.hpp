#ifndef MODEL_HPP
#define MODEL_HPP

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

enum DRAW_TYPE {
	ARRAY,
	INDEX
};

class Model {
	
protected:
	vector<vec3> m_positions;
	vector<unsigned int> m_indices;
	int m_lights[6] = { 0,0,0,0,0,0 };

	mat4* m_projection;
	mat4* m_eye_rbt;
	mat4* m_model_rbt;

	DRAW_TYPE m_draw_type;

	GLuint m_glsl_program_id;
	GLuint m_vertex_array_id;
	GLuint m_position_buffer_id;
	GLuint m_index_buffer_id;
public:
	// Class constructor
	Model();
	// Add vertex position in (x,y,z) coordinate in model space
	void AddPosition(float a_px, float a_py, float a_pz);
	// Add vertex position in (x,y,z) glm vector in model space
	void AddPosition(vec3 a_position);
	// Add vertex index to vector
	void AddIndex(unsigned int a_idx);
	// Set pointer of projection matrix for this model
	void SetProjection(mat4* p_projection);
	// Set pointer of camera rigid-body matrix for this model
	void SetEyeRbt(mat4* p_eye_rbt);
	// Set pointer of model matrix for this model
	void SetModelRbt(mat4* p_model_rbt);
	// Get Model Transformation
	mat4 GetModelRbt(void);
	// Switch lights
	void Switch(int i);
	// Set glsl program to this model
	void SetProgram(GLuint a_program_id);
	// Set a shader program for this model and transfer data to vertex buffers (abstract function)
	virtual void InitializeGLSL(DRAW_TYPE a_draw_type) = 0;
	// Draw function (abstract)
	virtual void Draw(void) = 0;
	// Clean up all resources (abstract)
	virtual void CleanUp(void) = 0;
};

#endif
