#include <iostream>
#include "node.hpp"

using namespace std;
using namespace glm;

Node::Node()
{
	m_parent = NULL;
	m_children = vector<Node*>();

	m_models = std::vector<Model*>();
	m_model_rbts = std::vector<mat4>();
	m_local_rbts = std::vector<mat4>();

	m_lines = std::vector<LineModel>();
	m_line_rbts = std::vector<mat4>();
	m_line_local_rbts = std::vector<mat4>();
}

void Node::AddModel(Model * a_model, mat4 a_local_rbt)
{
	m_model_rbts.push_back(m_rbt);
	m_local_rbts.push_back(a_local_rbt);
	m_models.push_back(a_model);
}

void Node::AddChild(Node* a_node)
{
	m_children.push_back(a_node);
	a_node->m_parent = this;
	a_node->m_level = m_level + 1;
}

// HW3: Only each models are doubled; number of children didn't increase.
void Node::SetLines(GLuint a_program, mat4 *a_projeciton, mat4 *a_eye)
{
	// Compute degree
	float radian_per_line = 2 * pi<float>() / m_children.size();

	for (int i = 0; i < m_children.size(); i++) // changed for scaled copies
	{
		LineModel a_line = LineModel();
		a_line.AddLine(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, line_height / 2.0, 0.0f));
		a_line.AddLine(vec3(0.0f, line_height /2.0, 0.0f), vec3(line_width / m_level, line_height / 2.0, 0.0f));
		a_line.AddLine(vec3(line_width / m_level, line_height / 2.0, 0.0f), vec3(line_width / m_level, line_height, 0.0f));

		a_line.InitializeGLSL(ARRAY);
		a_line.SetProjection(a_projeciton);
		a_line.SetEyeRbt(a_eye);
		a_line.SetProgram(a_program);

		m_lines.push_back(a_line);
		m_line_rbts.push_back(m_rbt);
		m_line_local_rbts.push_back(rotate(mat4(1.0f), i * radian_per_line, vec3(0.0f, 1.0f, 0.0f)));
	}
}

void Node::AddSpeed(float a_amount)
{
	m_rotation_speed += a_amount;
}

void Node::ResetSpeed(void)
{
	m_rotation_speed = 0.0f;
}

void Node::ApplyRotation(float elasped_time)
{
	m_frame_rbt = m_frame_rbt * rotate(mat4(1.0f), radians(m_rotation_speed * elasped_time), vec3(0.0f, 1.0f, 0.0f)); // M * R -> M'
	if(m_parent != NULL)
		m_rbt = m_parent->m_rbt * m_frame_rbt; // m_t = w_t * M, m_t * R -> m_t'
	else
		m_rbt = mat4(1.0f) * m_frame_rbt; // m_t = w_t * M, m_t * R -> m_t'
	UpdateChildrenFrame();
	UpdateObjectFrame();
}

void Node::LightSwitch(int i) // Turn on/off such light
{
	for(int j=0; j<m_models.size(); j+=2)
		(m_models[j])->Switch(i);
}

void Node::BlinnSwitch(void) // Blinn-phong or phong
{
	for (int j = 0; j < m_models.size(); j+=2)
		(m_models[j])->BlinnSwitch();
}

// HW3: Doesn't be affected: only number of models were doubled, children doesn't increased.
void Node::InitialChildrenFrame(void)
{
	// Compute degree
	float radian_per_line = 2 * pi<float>() / m_children.size();

	// Define child's frame
	for (int i = 0; i < m_children.size(); i++) // changed for scaled copies
	{
		// rotate degree and translate to position
		m_children[i]->m_frame_rbt = rotate(mat4(1.0f), i*radian_per_line, vec3(0.0f, 1.0f, 0.0f)) * translate(mat4(1.0f), vec3(line_width / m_level, line_height, 0.0f));
		m_children[i]->m_rbt = m_rbt * m_children[i]->m_frame_rbt;

		m_children[i]->InitialChildrenFrame();
	}
}

// HW3: Don't have to change since this change is applied for the whole node.
void Node::UpdateChildrenFrame(void) 
{
	// Define child's frame
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->m_rbt = m_rbt * m_children[i]->m_frame_rbt;
		m_children[i]->UpdateObjectFrame();
		m_children[i]->UpdateChildrenFrame();
	}
}

// HW3: Don't have to change since this change is applied for the whole node.
void Node::UpdateObjectFrame(void)
{
	for (int i = 0; i < m_models.size(); i++)
	{
		m_model_rbts[i] = m_rbt * m_local_rbts[i];
		m_models[i]->SetModelRbt(&m_model_rbts[i]);
	}

	for (int i = 0; i < m_lines.size(); i++)
	{
		m_line_rbts[i] = m_rbt * m_line_local_rbts[i];
		m_lines[i].SetModelRbt(&m_line_rbts[i]);
	}
}

// Planning to set depth buffer for shadow mapping
void Node::SetDepth(vector<mat4> shadowTransforms, float far_plane, vec3 lightPos) {
/* 	for (int i = 0; i < m_models.size(); i += 2) {
		m_models[i]->SetDepth(shadowTransforms, far_plane, lightPos);
	}
	*/
}

void Node::Draw(int outline, vec3 lightPos = vec3(0.0f, -3.0f, 3.0f))
{
	if (outline == 0) {
		for (int i = 0; i < m_models.size(); i += 2) {
			m_models[i]->Draw(lightPos);
		}
		for (int i = 0; i < m_lines.size(); i++)
		{
			m_lines[i].Draw(lightPos);
		}
	}
	else {
		for (int i = 1; i < m_models.size(); i += 2) {
			m_models[i]->Draw(lightPos);
		}
	}
}

void Node::CleanUp(void)
{
	for (int i = 0; i < m_models.size(); i++)
	{
		m_models[i]->CleanUp();
	}
	for (int i = 0; i < m_lines.size(); i++)
	{
		m_lines[i].CleanUp();
	}
	m_lines.clear();
	m_model_rbts.clear();
	m_local_rbts.clear();
	m_line_rbts.clear();
	m_line_local_rbts.clear();
}
