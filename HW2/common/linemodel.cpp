#include <iostream>
#include <vector>
#include <sstream>

#include "linemodel.hpp"

using namespace std;
using namespace glm;

LineModel::LineModel()
{
	m_positions = vector<vec3>();
	m_indices = vector<unsigned int>();
}

void LineModel::AddLine(vec3 a_start, vec3 a_end)
{
	m_positions.push_back(a_start);
	m_positions.push_back(a_end);
}

void LineModel::InitializeGLSL(DRAW_TYPE a_type)
{
	glUseProgram(m_glsl_program_id);
	glGenVertexArrays(1, &m_vertex_array_id);
	glBindVertexArray(m_vertex_array_id);

	glGenBuffers(1, &m_position_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_position_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*m_positions.size(), &m_positions[0], GL_STATIC_DRAW);
}

void LineModel::Draw(void)
{
	glUseProgram(m_glsl_program_id);
	GLint projection_id = glGetUniformLocation(m_glsl_program_id, "Projection");
	GLint eye_id = glGetUniformLocation(m_glsl_program_id, "Eye");
	GLint model_id = glGetUniformLocation(m_glsl_program_id, "ModelTransform");

	glUniformMatrix4fv(projection_id, 1, GL_FALSE, &(*(m_projection))[0][0]);
	glUniformMatrix4fv(eye_id, 1, GL_FALSE, &(*(m_eye_rbt))[0][0]);
	glUniformMatrix4fv(model_id, 1, GL_FALSE, &(*(m_model_rbt))[0][0]);

	glBindVertexArray(m_vertex_array_id);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_position_buffer_id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), ((GLvoid*)(0)));
	
	glDrawArrays(GL_LINES, 0, (GLsizei)m_positions.size());
}

void LineModel::CleanUp(void)
{
	// Clean up data structures
	m_positions.clear();
	glDisableVertexAttribArray(0);
	// Cleanup VBO and VAO (shader should be removed in the main function)
	glDeleteBuffers(1, &m_vertex_array_id);
	glDeleteVertexArrays(1, &m_vertex_array_id);
}
