#ifndef NODE_HPP
#define NODE_HPP

// The example solution for hierarchical frame
// n_f = parent_f * some matrix

#include <GL/glew.h>
#include <vector>
#include <common/geometry.hpp>

using namespace std;
using namespace glm;

class Node {

protected:
	Node * m_parent;
	vector<Node *> m_children;

	int m_level = 1;

	mat4 m_rbt = mat4(1.0f); // matrix with respect to world frame. n = w_t * m_rbt = parent * m_frame_rbt
	mat4 m_frame_rbt = mat4(1.0f);

	std::vector<Model*> m_models;
	std::vector<LineModel> m_lines;
	std::vector<mat4> m_model_rbts; // o = n * local_rbt
	std::vector<mat4> m_local_rbts;
	std::vector<mat4> m_line_rbts; // o = n * local_rbt
	std::vector<mat4> m_line_local_rbts;
	float m_rotation_speed = 0.0f;

	float line_width = 3.0f;
	float line_height = -2.0f;

public:
	
	Node(); // initialize vectors
	
	void AddModel(Model* a_model, mat4 a_local_rbt);
	void AddChild(Node* a_node);
	void SetLines(GLuint a_program, mat4 *a_projeciton, mat4 *a_eye);
	void AddSpeed(float a_amount);
	void ResetSpeed(void);
	void ApplyRotation(float a_elasped_time);
	// Switch lights
	void LightSwitch(int i);
	// Blinn-phong or phong
	void BlinnSwitch();
	void InitialChildrenFrame(void);
	void UpdateChildrenFrame(void);
	void UpdateObjectFrame(void);
	void Draw();
	void CleanUp(void);
};

#endif
