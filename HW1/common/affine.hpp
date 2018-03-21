#ifndef AFFINE_H
#define AFFINE_H
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

/*
 * An affine matrix A can be factored as A = TL. You need to fill up two function named 'get_linear' and 'get_translation'
 */

// TODO: Fill up get_linear function
// input: A (4 x 4 matrix)
// output: L (4 x 4 matrix)
inline mat4 get_linear(mat4 a_A)
{
	mat4 L = mat4(a_A[0], a_A[1], a_A[2], vec4(0.0f, 0.0f, 0.0f, 1.0f));
	return L;
}

// TODO: Fill up get_translation function
// input: A (4 x 4 matrix)
// output: T (4 x 4 matrix)
inline mat4 get_translation(mat4 a_A)
{
	mat4 T = translate(mat4(1.0f), vec3(a_A[3]));
	return T;
}
#endif
