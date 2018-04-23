#include <iostream>
#include <vector>
#include <sstream>

#include "model.hpp"
#include "shader.hpp"

using namespace std;
using namespace glm;

Model::Model()
{
	// Initialize model information
	m_positions = vector<vec3>();
	m_indices = vector<unsigned int>();
}

void Model::AddPosition(float a_px, float a_py, float a_pz)
{
	m_positions.push_back(vec3(a_px, a_py, a_pz));
}

void Model::AddPosition(vec3 a_position)
{
	m_positions.push_back(a_position);
}

void Model::AddIndex(unsigned int a_idx)
{
	m_indices.push_back(a_idx);
}

void Model::SetProjection(mat4* p_projection)
{
	m_projection = p_projection;
}

void Model::SetEyeRbt(mat4* p_eye_rbt)
{
	m_eye_rbt = p_eye_rbt;
}

void Model::SetModelRbt(mat4* p_model_rbt)
{
	m_model_rbt = p_model_rbt;
}

mat4 Model::GetModelRbt(void)
{
	return *m_model_rbt;
}

void Model::SetProgram(GLuint a_program_id)
{
	m_glsl_program_id = a_program_id;
}

void Model::Switch(int i)
{
	m_lights[i] = 1 - m_lights[i];
}