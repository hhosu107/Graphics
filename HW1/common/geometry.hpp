#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/model.hpp>

using namespace glm;

vec3 cubeVertices[8] = {
	vec3(-0.5, -0.5, 0.5),
	vec3(-0.5, 0.5, 0.5),
	vec3(0.5, 0.5, 0.5),
	vec3(0.5, -0.5, 0.5),
	vec3(-0.5, -0.5, -0.5),
	vec3(-0.5, 0.5, -0.5),
	vec3(0.5, 0.5, -0.5),
	vec3(0.5, -0.5, -0.5)
};

// 2. Define input primitives.
vec3 knotVertices[8] = {
	vec3(0.5, -2, 0.5),
	vec3(0.5, -2, -0.5),
	vec3(0.5, -1, -0.5),
	vec3(0.5, -1, 0.5),
	vec3(-0.5, 1, 0.5),
	vec3(-0.5, 1, -0.5),
	vec3(-0.5, 2, -0.5),
	vec3(-0.5, 2, 0.5)
};

// input: a_model (model's reference), a_a (positon of a), a_b (postion of b), a_c (position of c)
void ComputeNormal(Model &a_model, vec3 a_a, vec3 a_b, vec3 a_c)
{
	a_model.AddNormal(normalize(cross(a_b - a_a, a_c - a_a)));
	a_model.AddNormal(normalize(cross(a_b - a_a, a_c - a_a)));
	a_model.AddNormal(normalize(cross(a_b - a_a, a_c - a_a)));
}

// input: a_model (model's reference), a_a (positon of a), a_b (postion of b), a_c (position of c), a_d (position of d), a_color (color of quad)
void Quad(Model &a_model, int a_a, int a_b, int a_c, int a_d, vec3 a_color)
{
	/* a -- d
	 * |    |
	 * b -- c
	 */

	a_model.AddPosition(cubeVertices[a_a]);
	a_model.AddPosition(cubeVertices[a_b]);
	a_model.AddPosition(cubeVertices[a_c]);
	a_model.AddPosition(cubeVertices[a_a]);
	a_model.AddPosition(cubeVertices[a_c]);
	a_model.AddPosition(cubeVertices[a_d]);

	ComputeNormal(a_model, cubeVertices[a_a], cubeVertices[a_b], cubeVertices[a_c]);
	ComputeNormal(a_model, cubeVertices[a_a], cubeVertices[a_c], cubeVertices[a_d]);

	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
}

// 2. Define primitive geometries
// input: a_model (model's reference), a_a (positon of a), a_b (postion of b), a_c (position of c), a_d (position of d), a_color (color of quad)
void Knot(Model &a_model, int a_a, int a_b, int a_c, int a_d, vec3 a_color)
{
	/* a -- d
	 * |    |
	 * b -- c
	 */

	a_model.AddPosition(knotVertices[a_a]);
	a_model.AddPosition(knotVertices[a_b]);
	a_model.AddPosition(knotVertices[a_c]);
	a_model.AddPosition(knotVertices[a_a]);
	a_model.AddPosition(knotVertices[a_c]);
	a_model.AddPosition(knotVertices[a_d]);

	ComputeNormal(a_model, knotVertices[a_a], knotVertices[a_b], knotVertices[a_c]);
	ComputeNormal(a_model, knotVertices[a_a], knotVertices[a_c], knotVertices[a_d]);

	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
}

// input: a_model (model's reference), a_color (color of cube)
void InitDataCube(Model &a_model, vec3 a_color)
{
   /*   5 -- 6
    *  /|   /|
    * 1(4)-2 7
	* |    |/
	* 0 -- 3
	*/

	Quad(a_model, 1, 0, 3, 2, a_color);
	Quad(a_model, 2, 3, 7, 6, a_color);
	Quad(a_model, 3, 0, 4, 7, a_color);
	Quad(a_model, 6, 5, 1, 2, a_color);
	Quad(a_model, 4, 5, 6, 7, a_color);
	Quad(a_model, 5, 4, 0, 1, a_color);
}

// input: a_model (model's reference), a_colors (color of each face of cube)
void InitDataRubic(Model& a_model, vec3* a_colors)
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

// 2. Define input primitives.
void InitDataKnot(Model &a_model, vec3 a_color)
{
	/*     4 ------ 7   
	 *    /|       /|
	 * 0==(5)---3==(6)
	 * |/       |/
	 * 1 ------ 2
	 * (and also 0, 3, 6, 5 forms a rectangle)
	 */
	Knot(a_model, 0, 1, 2, 3, a_color);
	Knot(a_model, 0, 5, 6, 3, a_color);
	Knot(a_model, 5, 4, 7, 6, a_color);
	Knot(a_model, 4, 1, 2, 7, a_color);
}

void InitDataKnots(Model &a_model, vec3* a_color)
{
	/*     4 ------ 7
	*    /|       /|
	* 0==(5)---3==(6)
	* |/       |/
	* 1 ------ 2
	* (and also 0, 3, 6, 5 forms a rectangle)
	*/
	Knot(a_model, 0, 1, 2, 3, a_color[0]);
	Knot(a_model, 0, 5, 6, 3, a_color[1]);
	Knot(a_model, 5, 4, 7, 6, a_color[2]);
	Knot(a_model, 4, 1, 2, 7, a_color[3]);
}



// input: a_model (model's reference), a_color (color of sphere)
void InitDataSphere(Model &a_model, vec3 a_color)
{
	// TODO: Fill the vertex position, normal, color for your sphere object
	// Reference: Four ways to create a mesh for a sphere
	// https://medium.com/game-dev-daily/four-ways-to-create-a-mesh-for-a-sphere-d7956b825db4
}

#endif
