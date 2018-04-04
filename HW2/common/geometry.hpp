#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "common/colormodel.hpp"
#include <common/linemodel.hpp>

using namespace glm;

const vec3 vertices[8] = {
	vec3(-0.5, -0.5, 0.5),
	vec3(-0.5, 0.5, 0.5),
	vec3(0.5, 0.5, 0.5),
	vec3(0.5, -0.5, 0.5),
	vec3(-0.5, -0.5, -0.5),
	vec3(-0.5, 0.5, -0.5),
	vec3(0.5, 0.5, -0.5),
	vec3(0.5, -0.5, -0.5)
};

// input: a_model (model's reference), a_a (positon of a), a_b (postion of b), a_c (position of c)
inline void ComputeNormal(ColorModel &a_model, vec3 a_a, vec3 a_b, vec3 a_c)
{
	a_model.AddNormal(normalize(cross(a_b - a_a, a_c - a_a)));
	a_model.AddNormal(normalize(cross(a_b - a_a, a_c - a_a)));
	a_model.AddNormal(normalize(cross(a_b - a_a, a_c - a_a)));
}

// input: a_model (model's reference), a_a (positon of a), a_b (postion of b), a_c (position of c), a_d (position of d), a_color (color of quad)
inline void Quad(ColorModel &a_model, int a_a, int a_b, int a_c, int a_d, vec3 a_color)
{
	/* a -- d
	 * |    |
	 * b -- c
	 */

	a_model.AddPosition(vertices[a_a]);
	a_model.AddPosition(vertices[a_b]);
	a_model.AddPosition(vertices[a_c]);
	a_model.AddPosition(vertices[a_a]);
	a_model.AddPosition(vertices[a_c]);
	a_model.AddPosition(vertices[a_d]);

	ComputeNormal(a_model, vertices[a_a], vertices[a_b], vertices[a_c]);
	ComputeNormal(a_model, vertices[a_a], vertices[a_c], vertices[a_d]);

	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
}

inline void QuadVec(ColorModel &a_model, vec3 a_a, vec3 a_b, vec3 a_c, vec3 a_d, vec3 a_color)
{
	a_model.AddPosition(a_a);
	a_model.AddPosition(a_b);
	a_model.AddPosition(a_c);
	a_model.AddPosition(a_a);
	a_model.AddPosition(a_c);
	a_model.AddPosition(a_d);

	ComputeNormal(a_model, a_a, a_b, a_c);
	ComputeNormal(a_model, a_a, a_c, a_d);

	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
}

// input: a_model (model's reference), a_color (color of cube)
inline void InitDataCube(ColorModel &a_model, vec3 a_color)
{
   /*   5 -- 6
    *  /|   /|
    * 1(4)-2 7
	* |    |/
	* 0 -- 3
	*/

	Quad(a_model, 1, 0, 3, 2, a_color);
	Quad(a_model, 2, 3, 7, 6, a_color);
	Quad(a_model, 0, 4, 7, 3, a_color);
	Quad(a_model, 5, 1, 2, 6, a_color);
	Quad(a_model, 6, 7, 4, 5, a_color);
	Quad(a_model, 5, 4, 0, 1, a_color);
}

// input: a_model (model's reference), a_colors (color of each face of cube)
inline void InitDataRubic(ColorModel& a_model, vec3* a_colors)
{
	/*   5 -- 6
    *  /|   /|
    * 1(4)-2 7
	* |    |/
	* 0 -- 3
	*/
	Quad(a_model, 1, 0, 3, 2, a_colors[0]);
	Quad(a_model, 2, 3, 7, 6, a_colors[4]);
	Quad(a_model, 3, 0, 4, 7, a_colors[5]);
	Quad(a_model, 6, 5, 1, 2, a_colors[3]);
	Quad(a_model, 4, 5, 6, 7, a_colors[1]);
	Quad(a_model, 5, 4, 0, 1, a_colors[2]);
}


// input: a_model (model's reference), a_color (color of sphere)
inline void InitDataPyramid(ColorModel &a_model, vec3 a_color)
{
	vec3 p_vertices[5] = {
		vec3(0.0f,sqrt(3.0f)/2.0f,0.0f),
		vec3(-0.5f, 0.0f, 0.5f),
		vec3(-0.5f, 0.0f, -0.5f),
		vec3(0.5f, 0.0f, -0.5f),
		vec3(0.5f, 0.0f, 0.5f)
	};

	int p_indices[18] = {
		0,2,1,
		0,1,4,
		0,4,3,
		0,3,2,
		1,2,3,
		1,3,4
	};
	
	for (int i = 0; i < 18; i+=3)
	{
		int a_idx = p_indices[i];
		int b_idx = p_indices[i + 1];
		int c_idx = p_indices[i + 2];

		a_model.AddPosition(p_vertices[a_idx]);
		a_model.AddPosition(p_vertices[b_idx]);
		a_model.AddPosition(p_vertices[c_idx]);

		vec3 a_a = p_vertices[a_idx];
		vec3 a_b = p_vertices[b_idx];
		vec3 a_c = p_vertices[c_idx];

		a_model.AddNormal(normalize(cross(a_b - a_a, a_c - a_a)));
		a_model.AddNormal(normalize(cross(a_b - a_a, a_c - a_a)));
		a_model.AddNormal(normalize(cross(a_b - a_a, a_c - a_a)));

		a_model.AddColor(a_color);
		a_model.AddColor(a_color);
		a_model.AddColor(a_color);
	}
}

// input: a_model (model's reference), a_color (color of sphere)
inline void InitDataSphere(ColorModel &a_model, vec3 a_color)
{
	// TODO: Fill the vertex position, normal, color for your sphere object
	// Reference: Four ways to create a mesh for a sphere
	// https://medium.com/game-dev-daily/four-ways-to-create-a-mesh-for-a-sphere-d7956b825db4
	float radius = 0.5f;
	unsigned int rings = 30, sectors = 30;
	float R = 1.0f / (float)(rings - 1);
	float S = 1.0f / (float)(sectors - 1);
	float PI = pi<float>();

	for (unsigned int r = 0; r < rings; r++)
	{
		for (unsigned int s = 0; s < sectors; s++)
		{
			float x = (float)cos(2 * PI * s * S) * sin(PI * r * R);
			float y = (float)sin(-(PI / 2.0f) + PI * r * R);
			float z = (float)sin(2 * PI * s * S) * sin(PI * r * R);

			a_model.AddPosition(glm::vec3(x * radius, y * radius, z * radius));
			a_model.AddNormal(glm::vec3(x, y, z));
			a_model.AddColor(a_color);
		}
	}

	for (unsigned int r = 0; r < rings - 1; r++)
	{
		for (unsigned int s = 0; s < sectors - 1; s++)
		{
			a_model.AddIndex(r * sectors + (s + 1)); // 2
			a_model.AddIndex((r + 1) * sectors + s); // 3
			a_model.AddIndex((r + 1) * sectors + (s + 1)); // 4 
			a_model.AddIndex(r * sectors + s); // 1 
			a_model.AddIndex((r + 1) * sectors + s); // 3
			a_model.AddIndex(r * sectors + (s + 1)); // 2
		}
	}
}

// input: a_model (model's reference), a_color (color of sphere)
inline void InitDataCylinder(ColorModel &a_model, vec3 a_color)
{
	// TODO: Fill the vertex position, normal, color for your sphere object
	// Reference: Four ways to create a mesh for a sphere
	// https://medium.com/game-dev-daily/four-ways-to-create-a-mesh-for-a-sphere-d7956b825db4
	float radius = 0.5f;
	unsigned int rings = 30;
	float R = 1.0f / (float)(rings - 1);
	float PI = pi<float>();

	std::vector<vec3> upper_points = std::vector<vec3>();
	std::vector<vec3> bottom_points = std::vector<vec3>();

	for (unsigned int r = 0; r < rings; r++)
	{		
		float x = (float)cos(r * R * 2 * PI);
		float y = (float)0.5f;
		float z = (float)sin(r * R * 2 * PI);

		upper_points.push_back(vec3(radius * x, y, radius * z));
		bottom_points.push_back(vec3(radius * x, -y, radius * z));
	}

	for (int i = 0; i < upper_points.size(); i++)
	{
		vec3 upper_center = vec3(0.0f, 0.5f, 0.0f);
		vec3 a_p = upper_points[i];
		vec3 b_p;
		if (i + 1 == upper_points.size())
		{
			b_p = upper_points[0];
		}
		else {
			b_p = upper_points[i + 1];
		}
		a_model.AddPosition(upper_center);
		a_model.AddPosition(b_p);
		a_model.AddPosition(a_p);

		ComputeNormal(a_model, upper_center, b_p, a_p);

		a_model.AddColor(a_color);
		a_model.AddColor(a_color);
		a_model.AddColor(a_color);
	}

	for (int i = 0; i < bottom_points.size(); i++)
	{
		vec3 bottom_center = vec3(0.0f, -0.5f, 0.0f);
		vec3 a_p = bottom_points[i];
		vec3 b_p;
		if (i + 1 == bottom_points.size())
		{
			b_p = bottom_points[0];
		}
		else {
			b_p = bottom_points[i + 1];
		}
		a_model.AddPosition(bottom_center);
		a_model.AddPosition(a_p);
		a_model.AddPosition(b_p);

		ComputeNormal(a_model, bottom_center, a_p, b_p);

		a_model.AddColor(a_color);
		a_model.AddColor(a_color);
		a_model.AddColor(a_color);
	}

	for (int i = 0; i < upper_points.size(); i++)
	{
		vec3 a_a = upper_points[i];
		vec3 a_b = bottom_points[i];
		vec3 a_c;
		vec3 a_d;
		if (i + 1 == upper_points.size())
		{
			a_c = bottom_points[0];
			a_d = upper_points[0];
		}
		else {
			a_c = bottom_points[i + 1];
			a_d = upper_points[i + 1];
		}
		QuadVec(a_model, a_d, a_c, a_b, a_a, a_color);
	}
}


#endif
