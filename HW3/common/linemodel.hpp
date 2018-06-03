#ifndef LINEMODEL_HPP
#define LINEMODEL_HPP

#include <common/model.hpp>

using namespace std;
using namespace glm;

class LineModel : public Model {
protected:
	// vector<vec3> m_positions from Model
public:
	// Class constructor
	LineModel();
	// Add line function
	void AddLine(vec3 a_start, vec3 a_end);
	// Set a shader program for this model and transfer data to vertex buffers 
	void InitializeGLSL(DRAW_TYPE a_type);
	//void SetDepth(vector<mat4> shadowTransforms, float far_plane, vec3 lightPos);
	// Draw function
	void Draw(vec3 lightPos);
	// Clean up all resources
	void CleanUp(void);
};

#endif
