#include <iostream>
#include <vector>
#include <sstream>

#include "colormodel.hpp"

using namespace std;
using namespace glm;

ColorModel::ColorModel()
{
	m_positions = vector<vec3>();
	m_indices = vector<unsigned int>();
	m_normals = vector<vec3>();
	m_colors = vector<vec3>();
}

void ColorModel::AddNormal(float a_nx, float a_ny, float a_nz)
{
	m_normals.push_back(vec3(a_nx, a_ny, a_nz));
}

void ColorModel::AddNormal(vec3 a_normal)
{
	m_normals.push_back(a_normal);
}

void ColorModel::AddColor(float a_r, float a_g, float a_b)
{
	m_colors.push_back(vec3(a_r, a_g, a_b));
}

void ColorModel::AddColor(vec3 a_color)
{
	m_colors.push_back(a_color);
}

void ColorModel::InitializeGLSL(DRAW_TYPE a_draw_type)
{
	glUseProgram(m_glsl_program_id);
	m_draw_type = a_draw_type;

	glGenVertexArrays(1, &m_vertex_array_id);
	glBindVertexArray(m_vertex_array_id);

	glGenBuffers(1, &m_position_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_position_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*m_positions.size(), &m_positions[0], GL_STATIC_DRAW);

	if (m_draw_type == DRAW_TYPE::INDEX)
	{
		glGenBuffers(1, &m_index_buffer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
	}

	glGenBuffers(1, &m_normal_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_normal_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*m_normals.size(), &m_normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_color_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*m_colors.size(), &m_colors[0], GL_STATIC_DRAW);
}

void ColorModel::Draw(void)
{
	glUseProgram(m_glsl_program_id);
	GLint projection_id = glGetUniformLocation(m_glsl_program_id, "Projection");
	GLint eye_id = glGetUniformLocation(m_glsl_program_id, "Eye");
	GLint model_id = glGetUniformLocation(m_glsl_program_id, "ModelTransform");

	// matrices
	glUniformMatrix4fv(projection_id, 1, GL_FALSE, &(*(m_projection))[0][0]);
	glUniformMatrix4fv(eye_id, 1, GL_FALSE, &(*(m_eye_rbt))[0][0]);
	glUniformMatrix4fv(model_id, 1, GL_FALSE, &(*(m_model_rbt))[0][0]);

	glUniform1i(glGetUniformLocation(m_glsl_program_id, "light[0]"), m_lights[0]);
	glUniform1i(glGetUniformLocation(m_glsl_program_id, "light[1]"), m_lights[1]);
	glUniform1i(glGetUniformLocation(m_glsl_program_id, "light[2]"), m_lights[2]);
	glUniform1i(glGetUniformLocation(m_glsl_program_id, "light[3]"), m_lights[3]);
	glUniform1i(glGetUniformLocation(m_glsl_program_id, "light[4]"), m_lights[4]);
	glUniform1i(glGetUniformLocation(m_glsl_program_id, "light[5]"), m_lights[5]);

	// material properties
	glUniform1f(glGetUniformLocation(m_glsl_program_id, "material.alpha"), 3.0f);

	// directional light (R, G, B)
	glUniform1f(glGetUniformLocation(m_glsl_program_id, "d"), 0.4f);
	glUniform1f(glGetUniformLocation(m_glsl_program_id, "s"), 0.6f);
	glUniform1f(glGetUniformLocation(m_glsl_program_id, "blinn"), blinn);

	glUniform3f(glGetUniformLocation(m_glsl_program_id, "directional[0].dir"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(m_glsl_program_id, "directional[0].color"), 1.0f, 0.0f, 0.0f);

	glUniform3f(glGetUniformLocation(m_glsl_program_id, "directional[1].dir"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(m_glsl_program_id, "directional[1].color"), 0.0f, 1.0f, 0.0f);

	glUniform3f(glGetUniformLocation(m_glsl_program_id, "directional[2].dir"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(m_glsl_program_id, "directional[2].color"), 0.0f, 0.0f, 1.0f);

	// point light
	glUniform3f(glGetUniformLocation(m_glsl_program_id, "point[0].pos"), 2.0f, 3.0f, -2.0f);
	glUniform3f(glGetUniformLocation(m_glsl_program_id, "point[0].color"), 0.8f, 0.0f, 0.8f);

	glUniform1f(glGetUniformLocation(m_glsl_program_id, "point[0].a"), 1.0f);
	glUniform1f(glGetUniformLocation(m_glsl_program_id, "point[0].b"), 0.1f);
	glUniform1f(glGetUniformLocation(m_glsl_program_id, "point[0].c"), 0.1f);

	glUniform3f(glGetUniformLocation(m_glsl_program_id, "point[1].pos"), 0.0f, -3.0f, 3.0f);
	glUniform3f(glGetUniformLocation(m_glsl_program_id, "point[1].color"), 1.0f, 1.0f, 1.0f);

	glUniform1f(glGetUniformLocation(m_glsl_program_id, "point[1].a"), 1.0f);
	glUniform1f(glGetUniformLocation(m_glsl_program_id, "point[1].b"), 0.0f);
	glUniform1f(glGetUniformLocation(m_glsl_program_id, "point[1].c"), 0.0f);

	glUniform3f(glGetUniformLocation(m_glsl_program_id, "spot.pos"), -1.0f, -4.0f, 10.0f);
	glUniform3f(glGetUniformLocation(m_glsl_program_id, "spot.color"), 0.7f, 0.7f, 0.7f);

	glUniform3f(glGetUniformLocation(m_glsl_program_id, "spot.dir"), 0.1f, 0.2f, -1.0f);
	glUniform1f(glGetUniformLocation(m_glsl_program_id, "spot.cutoff"), atan(0.3f));

	glUniform1f(glGetUniformLocation(m_glsl_program_id, "spot.a"), 1.0f);
	glUniform1f(glGetUniformLocation(m_glsl_program_id, "spot.b"), 0.0f);
	glUniform1f(glGetUniformLocation(m_glsl_program_id, "spot.c"), 0.01f);
	


	glBindVertexArray(m_vertex_array_id);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_position_buffer_id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), ((GLvoid*)(0)));

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_normal_buffer_id);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), ((GLvoid*)(0)));

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer_id);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), ((GLvoid*)(0)));

	if (m_draw_type == DRAW_TYPE::ARRAY)
	{
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_positions.size());
	}
	else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_id);
		glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, ((GLvoid *)0));
	}
}

void ColorModel::CleanUp(void)
{
	// Clean up data structures
	m_positions.clear();
	m_indices.clear();
	m_normals.clear();
	m_colors.clear();
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// Cleanup VBO and VAO (shader should be removed in the main function)
	glDeleteBuffers(1, &m_vertex_array_id);
	glDeleteBuffers(1, &m_normal_buffer_id);
	glDeleteBuffers(1, &m_color_buffer_id);
	if (m_draw_type == DRAW_TYPE::INDEX) glDeleteBuffers(1, &m_index_buffer_id);
	glDeleteVertexArrays(1, &m_vertex_array_id);
}
