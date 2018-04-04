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
	// Draw function
	void Draw(void);
	// Clean up all resources
	void CleanUp(void);
};

#endif
