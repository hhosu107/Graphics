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

vec3 octaVertices[6] = {
	vec3(-0.5, 0, 0.5),
	vec3(0.5, 0, 0.5),
	vec3(0.5, 0, -0.5),
	vec3(-0.5, 0, -0.5),
	vec3(0, 0.7, 0),
	vec3(0, -0.7, 0)
};

// 2. Define input primitives.
vec3 knotVertices[10] = {
	vec3(0.5, -0.5, 0.5),
	vec3(0.5, -0.5, -0.5),
	vec3(0.5, 0.5, -0.5),
	vec3(0.5, 0.5, 0.5),
	vec3(-0.5, -0.5, 0.5),
	vec3(-0.5, -0.5, -0.5),
	vec3(-0.5, 0.5, -0.5),
	vec3(-0.5, 0.5, 0.5),
	vec3(0, -0.5, 0),
	vec3(0, 0.5, 0)
};

vec3 pyramidVertices[5] = {
	vec3(-0.5, -0.5, 0.5),
	vec3(0.5, -0.5, 0.5),
	vec3(0.5, -0.5, -0.5),
	vec3(-0.5, -0.5, -0.5),
	vec3(0, 0.5, 0)
};

// input: a_model (model's reference), a_a (positon of a), a_b (postion of b), a_c (position of c)
void ComputeNormal(Model &a_model, vec3 a_a, vec3 a_b, vec3 a_c)
{
	a_model.AddNormal(normalize(cross(a_b - a_a, a_c - a_a)));
	a_model.AddNormal(normalize(cross(a_b - a_a, a_c - a_a)));
	a_model.AddNormal(normalize(cross(a_b - a_a, a_c - a_a)));
}

// 4. Implement hierarchical frames: draw lines between parent and child dynamically
void Line(Model &a_model, vec3 a_a, vec3 a_b) {
	a_model.AddPosition(a_a);
	a_model.AddPosition(a_b);
	//a_model.AddPosition(a_a);

	a_model.AddNormal(normalize(vec3(0.0, -1.0, 1.0)));
	a_model.AddNormal(normalize(vec3(0.0, -1.0, 1.0)));
	//a_model.AddNormal(normalize(vec3(0.0, -1.0, 1.0)));

	a_model.AddColor(vec3(1.0, 1.0, 1.0));
	a_model.AddColor(vec3(1.0, 1.0, 1.0));
	//a_model.AddColor(vec3(1.0, 1.0, 1.0));
}

// 2. Define primitive geometries
// input: a_model (model's reference), a_a (positon of a), a_b (postion of b), a_c (position of c), a_color (color of quad), transformation (initial transformation for models)
// 7. Creativity: can apply transformation on the model vertices itself when initializing.
void KnotTri(Model &a_model, int a_a, int a_b, int a_c, mat4 transformation, vec3 a_color)
{
	/*    a
	*   / \
	*  b---c
	*/
	a_model.AddPosition(vec3(transformation * vec4(knotVertices[a_a], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(knotVertices[a_b], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(knotVertices[a_c], 1.0f)));

	ComputeNormal(a_model, vec3(transformation * vec4(knotVertices[a_a], 1.0f)), vec3(transformation * vec4(knotVertices[a_b], 1.0f)), vec3(transformation * vec4(knotVertices[a_c], 1.0f)));

	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
}

// input: a_model (model's reference), a_a (positon of a), a_b (postion of b), a_c (position of c), a_color (color of quad), transformation (initial transformation for models)
// 7. Creativity: can apply transformation on the model vertices itself when initializing.
void Tri(Model &a_model, int a_a, int a_b, int a_c, mat4 transformation, vec3 a_color)
{
	/*    a
	 *   / \
	 *  b---c
	 */
	a_model.AddPosition(vec3(transformation * vec4(pyramidVertices[a_a], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(pyramidVertices[a_b], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(pyramidVertices[a_c], 1.0f)));

	ComputeNormal(a_model, vec3(transformation * vec4(pyramidVertices[a_a], 1.0f)), vec3(transformation * vec4(pyramidVertices[a_b], 1.0f)), vec3(transformation * vec4(pyramidVertices[a_c], 1.0f)));

	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
}

// input: a_model (model's reference), a_a (positon of a), a_b (postion of b), a_c (position of c), a_color (color of quad), transformation (initial transformation for models)
// 7. Creativity: can apply transformation on the model vertices itself when initializing.
void OctaTri(Model &a_model, int a_a, int a_b, int a_c, mat4 transformation, vec3 a_color)
{
	/*    a
	 *   / \
	 *  b---c
	 */
	a_model.AddPosition(vec3(transformation * vec4(octaVertices[a_a], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(octaVertices[a_b], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(octaVertices[a_c], 1.0f)));

	ComputeNormal(a_model, vec3(transformation * vec4(octaVertices[a_a], 1.0f)), vec3(transformation * vec4(octaVertices[a_b], 1.0f)), vec3(transformation * vec4(octaVertices[a_c], 1.0f)));

	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
}

// input: a_model (model's reference), a_a (positon of a), a_b (postion of b), a_c (position of c), a_d (position of d), a_color (color of quad), transformation (initial transformation for models)
// 7. Creativity: can apply transformation on the model vertices itself when initializing.
void Quad(Model &a_model, int a_a, int a_b, int a_c, int a_d, mat4 transformation, vec3 a_color)
{
	/* a -- d
	 * |    |
	 * b -- c
	 */

	a_model.AddPosition(vec3(transformation * vec4(cubeVertices[a_a], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(cubeVertices[a_b], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(cubeVertices[a_c], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(cubeVertices[a_a], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(cubeVertices[a_c], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(cubeVertices[a_d], 1.0f)));

	ComputeNormal(a_model, vec3(transformation * vec4(cubeVertices[a_a], 1.0f)), vec3(transformation * vec4(cubeVertices[a_b], 1.0f)), vec3(transformation * vec4(cubeVertices[a_c], 1.0f)));
	ComputeNormal(a_model, vec3(transformation * vec4(cubeVertices[a_a], 1.0f)), vec3(transformation * vec4(cubeVertices[a_c], 1.0f)), vec3(transformation * vec4(cubeVertices[a_d], 1.0f)));

	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
}

// input: a_model (model's reference), a_a (positon of a), a_b (postion of b), a_c (position of c), a_d (position of d), a_color (color of quad), transformation (initial transformation for models)
// 7. Creativity: can apply transformation on the model vertices itself when initializing.
void Knot(Model &a_model, int a_a, int a_b, int a_c, int a_d, mat4 transformation, vec3 a_color)
{
	/* a -- d
	 * |    |
	 * b -- c
	 */

	a_model.AddPosition(vec3(transformation * vec4(knotVertices[a_a], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(knotVertices[a_b], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(knotVertices[a_c], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(knotVertices[a_a], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(knotVertices[a_c], 1.0f)));
	a_model.AddPosition(vec3(transformation * vec4(knotVertices[a_d], 1.0f)));

	ComputeNormal(a_model, vec3(transformation * vec4(knotVertices[a_a], 1.0f)), vec3(transformation * vec4(knotVertices[a_b], 1.0f)), vec3(transformation * vec4(knotVertices[a_c], 1.0f)));
	ComputeNormal(a_model, vec3(transformation * vec4(knotVertices[a_a], 1.0f)), vec3(transformation * vec4(knotVertices[a_c], 1.0f)), vec3(transformation * vec4(knotVertices[a_d], 1.0f)));

	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
}

// input: a_model (model's reference), a_color (color of cube), transformation (initial transformation for models)
void InitDataCube(Model &a_model, mat4 transformation, vec3 a_color)
{
   /*   5 -- 6
    *  /|   /|
    * 1(4)-2 7
	* |    |/
	* 0 -- 3
	*/

	Quad(a_model, 1, 0, 3, 2, transformation, a_color);
	Quad(a_model, 2, 3, 7, 6, transformation, a_color);
	Quad(a_model, 3, 0, 4, 7, transformation, a_color);
	Quad(a_model, 6, 5, 1, 2, transformation, a_color);
	Quad(a_model, 4, 5, 6, 7, transformation, a_color);
	Quad(a_model, 5, 4, 0, 1, transformation, a_color);
}

// input: a_model (model's reference), a_color (color of cube), transformation (initial transformation for models)
void InitDataRubic(Model& a_model, mat4 transformation, vec3* a_colors)
{
	/*   5 -- 6
    *  /|   /|
    * 1(4)-2 7
	* |/   |/
	* 0 -- 3
	*/
	Quad(a_model, 1, 0, 3, 2, transformation, a_colors[0]);
	Quad(a_model, 2, 3, 7, 6, transformation, a_colors[4]);
	Quad(a_model, 3, 0, 4, 7, transformation, a_colors[5]);
	Quad(a_model, 6, 5, 1, 2, transformation, a_colors[3]);
	Quad(a_model, 4, 5, 6, 7, transformation, a_colors[1]);
	Quad(a_model, 5, 4, 0, 1, transformation, a_colors[2]);
}

// input: a_model (model's reference), a_color (color of cube), transformation (initial transformation for models)
void InitDataKnot(Model &a_model, mat4 transformation, vec3 a_color)
{
	/*     4 ------ 7   
	 *    /|       /|
	 * 0=8(5)---3=9(6)
	 * |/       |/
	 * 1 ------ 2
	 */
	Knot(a_model, 0, 1, 2, 3, transformation, a_color);
	KnotTri(a_model, 3, 2, 9, transformation, a_color);
	KnotTri(a_model, 9, 6, 7, transformation, a_color);
	Knot(a_model, 8, 0, 3, 9, transformation, a_color);
	Knot(a_model, 4, 8, 9, 7, transformation, a_color);

	Knot(a_model, 5, 4, 7, 6, transformation, a_color);
	KnotTri(a_model, 4, 5, 8, transformation, a_color);
	KnotTri(a_model, 8, 1, 0, transformation, a_color);
	Knot(a_model, 9, 8, 5, 6, transformation, a_color);
	Knot(a_model, 9, 2, 1, 8, transformation, a_color);
}

// input: a_model (model's reference), a_color (color of cube), transformation (initial transformation for models)
void InitDataKnot(Model &a_model, mat4 transformation, vec3* a_colors)
{
	/*     4 ------ 7
	 *    /|       /|
	 * 0=8(5)---3=9(6)
	 * |/       |/
	 * 1 ------ 2
	 */
	Knot(a_model, 0, 1, 2, 3, transformation, a_colors[0]);
	KnotTri(a_model, 3, 2, 9, transformation, a_colors[1]);
	KnotTri(a_model, 9, 6, 7, transformation, a_colors[2]);
	Knot(a_model, 8, 0, 3, 9, transformation, a_colors[3]);
	Knot(a_model, 4, 8, 9, 7, transformation, a_colors[4]);

	Knot(a_model, 5, 4, 7, 6, transformation, a_colors[5]);
	KnotTri(a_model, 4, 5, 8, transformation, a_colors[6]);
	KnotTri(a_model, 8, 1, 0, transformation, a_colors[7]);
	Knot(a_model, 9, 8, 5, 6, transformation, a_colors[8]);
	Knot(a_model, 9, 2, 1, 8, transformation, a_colors[9]);
}

// input: a_model (model's reference), a_color (color of cube), transformation (initial transformation for models)
void InitDataPyramid(Model &a_model, mat4 transformation, vec3 a_color)
{
	/*        4
	 *		 /|\  .
	 *		/ 3-\----2
	 *	   /.    \ .
	 *	  0-------1
	 */
	Tri(a_model, 1, 0, 3, transformation, a_color);
	Tri(a_model, 1, 3, 2, transformation, a_color);
	Tri(a_model, 4, 0, 1, transformation, a_color);
	Tri(a_model, 4, 1, 2, transformation, a_color);
	Tri(a_model, 3, 4, 2, transformation, a_color);
	Tri(a_model, 4, 3, 0, transformation, a_color);
}

// input: a_model (model's reference), a_color (color of cube), transformation (initial transformation for models)
void InitDataPyramids(Model &a_model, mat4 transformation, vec3* a_color)
{
	/*        4
	 *		 /|\  .
	 *		/ 3-\----2
	 *	   /.    \ .
	 *	  0-------1
	 */
	Tri(a_model, 1, 0, 3, transformation, a_color[0]);
	Tri(a_model, 1, 3, 2, transformation, a_color[0]);
	Tri(a_model, 4, 0, 1, transformation, a_color[1]);
	Tri(a_model, 4, 1, 2, transformation, a_color[2]);
	Tri(a_model, 3, 4, 2, transformation, a_color[3]);
	Tri(a_model, 4, 3, 0, transformation, a_color[4]);
}

// input: a_model (model's reference), a_color (color of cube), transformation (initial transformation for models)
void InitDataOcta(Model &a_model, mat4 transformation, vec3 a_color)
{
	/*        4
	*		 /|\  .
	*		/(3)\---(2)
	*	   /. |  \ .
	*	  0---|---1
	*      \  |  /
	*       \ | /
	*        \|/
	*         5
	*(also from 2 to 5)
	*/
	OctaTri(a_model, 4, 0, 1, transformation, a_color);
	OctaTri(a_model, 4, 1, 2, transformation, a_color);
	OctaTri(a_model, 3, 4, 2, transformation, a_color);
	OctaTri(a_model, 4, 3, 0, transformation, a_color);
	OctaTri(a_model, 1, 0, 5, transformation, a_color);
	OctaTri(a_model, 1, 5, 2, transformation, a_color);
	OctaTri(a_model, 5, 3, 2, transformation, a_color);
	OctaTri(a_model, 5, 0, 3, transformation, a_color);
}

// input: a_model (model's reference), a_color (color of cube), transformation (initial transformation for models)
void InitDataOcta(Model &a_model, mat4 transformation, vec3* a_color)
{
	/*        4
	 *		 /|\   .
	 *		/(3)\-----(2)
	 *	   /. |  \  .
	 *	  0---|---1
	 *     \  |  /
	 *      \ | /
	 *       \|/
	 *        5
	 *(also from 2 to 5)
	 */
	OctaTri(a_model, 4, 0, 1, transformation, a_color[0]);
	OctaTri(a_model, 4, 1, 2, transformation, a_color[1]);
	OctaTri(a_model, 3, 4, 2, transformation, a_color[2]);
	OctaTri(a_model, 4, 3, 0, transformation, a_color[3]);
	OctaTri(a_model, 1, 0, 5, transformation, a_color[4]);
	OctaTri(a_model, 1, 5, 2, transformation, a_color[5]);
	OctaTri(a_model, 5, 3, 2, transformation, a_color[6]);
	OctaTri(a_model, 5, 0, 3, transformation, a_color[7]);
}

// input: a_model (model's reference), a_color (color of sphere)
void InitDataSphere(Model &a_model, vec3 a_color)
{
	// TODO: Fill the vertex position, normal, color for your sphere object
	// Reference: Four ways to create a mesh for a sphere
	// https://medium.com/game-dev-daily/four-ways-to-create-a-mesh-for-a-sphere-d7956b825db4
}

#endif
