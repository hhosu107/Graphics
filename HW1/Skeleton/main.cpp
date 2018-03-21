// Include standard headers
#include <iostream>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// Include common libraries
#include <common/geometry.hpp>
#include <common/model.hpp>
#include <common/affine.hpp>

using namespace std;
using namespace glm;

#define DEBUG 1

// Global variable
GLFWwindow* g_window;
float g_window_width = 1024.f;
float g_window_height = 768.f;
int g_framebuffer_width = 1024;
int g_framebuffer_height = 768;
float g_fov = 45.f; // 45-degree of fov
float g_fovy = g_fov; // y-angle of fov

std::vector<Model> g_models;
std::vector<mat4> g_model_rbts; // 4. Implement hierarchical frames: will contain the relative transform when initialized.
std::vector<float> g_model_rotation_speed; // 5. Rotation speed.
mat4 model_view;
std::vector<mat4> model_view_stack; // 4. Implement hierarchical frames: using model_view_stack.end() * (g_model_rbts = g_model_rbts[i] * rotation(g_model_rotation_speed * (1 / fps))), do model_view_stack.push_back().

bool pickObject = false; // 5. Controls whether or not the object can be picked by 'o' command
int rot_ptr; // 5. Index for the current rotation manipulation. If pickObject is false, it shouldn't be changed even if we select 'o' command
vec3 dir_light = vec3(0.0f, -1.0f, 1.0f); // Pull out the directional light


float lastTime; // saves the current time
float currentTime;

// Transformations
mat4 g_projection;
// Camera model property
mat4 g_eye_rbt;
// World model property
mat4 g_world_rbt = mat4(1.0f);
// 7. Creativity: Will rotate the world model property itself according to the x-axis
float g_world_rbt_rotation_speed_x = 0.0f;
float g_world_rbt_rotation_speed_z = 0.0f;

// Event functions
static void WindowSizeCallback(GLFWwindow*, int, int);
static void MouseButtonCallback(GLFWwindow*, int, int, int);
static void CursorPosCallback(GLFWwindow*, double, double);
static void KeyboardCallback(GLFWwindow*, int, int, int, int);
void UpdateFovy(void);
void CleanUp(void);

// Helper function: Update the vertical field-of-view(float fovy in global)
void UpdateFovy()
{
	if (g_framebuffer_width >= g_framebuffer_height)
	{
		g_fovy = g_fov;
	}
	else {
		const float RAD_PER_DEG = 0.5f * glm::pi<float>() / 180.0f;
		g_fovy = (float)atan2(sin(g_fov * RAD_PER_DEG) * ((float)g_framebuffer_height / (float)g_framebuffer_width), cos(g_fov * RAD_PER_DEG)) / RAD_PER_DEG;
	}
}

void CleanUp()
{
	for (int i = 0; i < g_models.size(); i++)
	{
		g_models[i].CleanUp();
	}
	g_models.clear();
	g_models.shrink_to_fit();

	g_model_rbts.clear();
	g_model_rbts.shrink_to_fit();

	g_model_rotation_speed.clear();
	g_model_rotation_speed.shrink_to_fit();

	model_view_stack.clear();
	model_view_stack.shrink_to_fit();
}

// TODO: Modify GLFW window resized callback function
static void WindowSizeCallback(GLFWwindow* a_window, int a_width, int a_height)
{
	// Get resized size and set to current window
	g_window_width = (float)a_width;
	g_window_height = (float)a_height;

	// glViewport accept pixel size, please use glfwGetFramebufferSize rather than window size.
	// window size != framebuffer size
	glfwGetFramebufferSize(a_window, &g_framebuffer_width, &g_framebuffer_height);
	glViewport(0, 0, (GLsizei)g_framebuffer_width, (GLsizei)g_framebuffer_height);
	
	// Update projection matrix
	g_projection = perspective(g_fov, ((float)g_framebuffer_width / (float)g_framebuffer_height), 0.1f, 100.0f);
}

// TODO: Fill up GLFW mouse button callback function
static void MouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods)
{
	//example code for get x position and y position of mouse click
	if (a_button == GLFW_MOUSE_BUTTON_LEFT && a_action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(a_window, &xpos, &ypos);
		xpos = xpos / ((double)g_window_width) * ((double)g_framebuffer_width);
		ypos = ypos / ((double)g_window_height) * ((double)g_framebuffer_height);
	}
}

// TODO: Fill up GLFW cursor position callback function
static void CursorPosCallback(GLFWwindow* a_window, double a_xpos, double a_ypos)
{

}

static void KeyboardCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
{
	if (a_action == GLFW_PRESS)
	{
		switch (a_key)
		{
		case GLFW_KEY_H:
			cout << "CS580 Homework Assignment 1" << endl;
			cout << "keymaps:" << endl;
			cout << "h\t\t Help command" << endl;
			cout << "p\t\t Enable/Disable picking: o doesn't change the object while the picking is disabled" << endl;
			cout << "o\t\t Pick the next object to manipulate: using BFS" << endl;
			cout << "n\t\t Change the rotation speed of the current object by -10 degree per sec according to the local y-axis" << endl;
			cout << "m\t\t Change the rotation speed of the current object by 10 degree per sec according to the local y-axis" << endl;
			cout << "r\t\t Reset the rotation speed of the current object by 0 degree per sec according to the local y-axis" << endl;
			cout << "j\t\t Change the rotation speed of the camera by -10 degree per sec according to the local x-axis" << endl;
			cout << "l\t\t Change the rotation speed of the camera by 10 degree per sec according to the local x-axis" << endl;
			cout << "k\t\t Reset the rotation speed of the camera by 0 degree per sec according to the local x-axis" << endl;
			cout << "y\t\t Change the rotation speed of the camera by -10 degree per sec according to the local z-axis" << endl;
			cout << "i\t\t Change the rotation speed of the camera by 10 degree per sec according to the local z-axis" << endl;
			cout << "u\t\t Reset the rotation speed of the camera by 0 degree per sec according to the local z-axis" << endl;
			break;
		case GLFW_KEY_P:
			pickObject = !pickObject;
			break;
		case GLFW_KEY_O:
			if (pickObject) rot_ptr = (rot_ptr + 1) % g_models.size();
			break;
		case GLFW_KEY_N:
			if (pickObject) {
				g_model_rotation_speed[rot_ptr] -= 10;
			}
			break;
		case GLFW_KEY_M:
			if (pickObject) {
				g_model_rotation_speed[rot_ptr] += 10;
			}
			break;
		case GLFW_KEY_R:
			if (pickObject) {
				g_model_rotation_speed[rot_ptr] = 0;
			}
			break;
		case GLFW_KEY_J:
			g_world_rbt_rotation_speed_x -= 10;
			break;
		case GLFW_KEY_L:
			g_world_rbt_rotation_speed_x += 10;
			break;
		case GLFW_KEY_K:
			g_world_rbt_rotation_speed_x = 0;
			break;
		case GLFW_KEY_Y:
			g_world_rbt_rotation_speed_z -= 10;
			break;
		case GLFW_KEY_I:
			g_world_rbt_rotation_speed_z += 10;
			break;
		case GLFW_KEY_U:
			g_world_rbt_rotation_speed_z = 0;
			break;
		default:
			break;
		}
	}
}

void traverse(int i) {
	if (i < 0 || i >= g_models.size()) return;
	model_view_stack.push_back(model_view);
	g_model_rbts[i] = get_translation(g_model_rbts[i]) * rotate(mat4(1.0f), (currentTime - lastTime)*g_model_rotation_speed[i], vec3(0.0f, 1.0f, 0.0f)) * get_linear(g_model_rbts[i]) ;
	mat4 temp_model_view = model_view * get_translation(g_model_rbts[i]) * get_linear(g_model_rbts[i]);
	// Draw line btwn parent and child first. If the node is the root, then the line will starts from the origin point.
	Model line = Model();
	Line(line, vec3(model_view[3]), vec3(temp_model_view[3]));
	line.InitializeGLSL(DRAW_TYPE::LINE, "VertexShader.glsl", "LineFragmentShader.glsl");
	line.SetProjection(&g_projection);
	line.SetEyeRbt(&g_eye_rbt);

	mat4 line_rbt = mat4(1.0);
	line.SetModelRbt(&line_rbt);
	line.SetDirectionalLight(dir_light);

	line.Draw();
	// Line draw end
	
	model_view = temp_model_view;
	mat4 temp_g_model_rbt = g_model_rbts[i];
	
	g_model_rbts[i] = model_view;
	g_models[i].SetModelRbt(&g_model_rbts[i]);

	if (pickObject && i == rot_ptr) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		g_models[i].Draw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
		g_models[i].Draw();
	g_model_rbts[i] = temp_g_model_rbt;
	
	if (2 * i + 1 < g_models.size()) traverse(2 * i + 1);
	if (2 * i + 2 < g_models.size()) traverse(2 * i + 2);
	model_view = model_view_stack.back();
	model_view_stack.pop_back();
}

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Open a window and create its OpenGL context
	g_window = glfwCreateWindow( 1024, 768, "Homework 1: 20173245 - Chansu Park", NULL, NULL);

	if( g_window == NULL ){
		fprintf( stderr, "Failed to open GLFW window." );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(g_window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetWindowSizeCallback(g_window, WindowSizeCallback);
	glfwSetMouseButtonCallback(g_window, MouseButtonCallback);
	glfwSetCursorPosCallback(g_window, CursorPosCallback);
	glfwSetKeyCallback(g_window, KeyboardCallback);

	// Clear with black backround
	glClearColor((GLclampf)0.0f, (GLclampf)0.0f, (GLclampf)0.0f, (GLclampf) 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Enable culling
	glEnable(GL_CULL_FACE);
	// Backface culling
	glCullFace(GL_BACK);

	// Setting viewport
	// glViewport accept pixel size, please use glfwGetFramebufferSize rather than window size.
	// window size != framebuffer size
	glfwGetFramebufferSize(g_window, &g_framebuffer_width, &g_framebuffer_height);
	glViewport(0, 0, (GLsizei)g_framebuffer_width, (GLsizei)g_framebuffer_height);

	g_projection = perspective(g_fov, ((float)g_framebuffer_width / (float)g_framebuffer_height), 0.1f, 100.0f);

	g_eye_rbt = translate(mat4(1.0f), vec3(0.0, -1.0f, 15.0));

	// initialize models
	g_models = std::vector<Model>();
	g_model_rbts = std::vector<mat4>();
	g_model_rotation_speed = std::vector<float>(); // 5. Will contain the current rotation speed.

	// initialize directional light
	dir_light = vec3(0.0f, -1.0f, 1.0f);

	int model_count = 0;

	// 2. Using other primitives
	Model first_knot = Model();
	InitDataKnot(first_knot, vec3(1.0f, 1.0f, 1.0f)); // Pure infinity
	first_knot.InitializeGLSL(DRAW_TYPE::ARRAY, "VertexShader.glsl", "FragmentShader.glsl");
	first_knot.SetProjection(&g_projection);
	first_knot.SetEyeRbt(&g_eye_rbt);

	mat4 first_knot_rbt = translate(g_world_rbt, vec3(0.0f, 1.0f, 0.0f));
	g_model_rbts.push_back(first_knot_rbt);
	g_model_rotation_speed.push_back(0);
	first_knot.SetModelRbt(&g_model_rbts[model_count++]);
	first_knot.SetDirectionalLight(dir_light);

	g_models.push_back(first_knot);

	Model pyramid = Model();
	InitDataPyramid(pyramid, vec3(0.776f, 0.541f, 0.071f)); // The color of the pyramid
	pyramid.InitializeGLSL(DRAW_TYPE::ARRAY, "VertexShader.glsl", "FragmentShader.glsl");
	pyramid.SetProjection(&g_projection);
	pyramid.SetEyeRbt(&g_eye_rbt);

	mat4 pyramid_rbt = translate(g_world_rbt, vec3(-3.0f, -3.0f, 0.0f));
	g_model_rbts.push_back(pyramid_rbt);
	g_model_rotation_speed.push_back(0);
	pyramid.SetModelRbt(&g_model_rbts[model_count++]);
	pyramid.SetDirectionalLight(dir_light);

	g_models.push_back(pyramid);

	Model cube = Model();
	vec3 cubeColor[6] = { vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f) }; // colorful cube
	InitDataRubic(cube, cubeColor);
	cube.InitializeGLSL(DRAW_TYPE::ARRAY, "VertexShader.glsl", "FragmentShader.glsl");
	cube.SetProjection(&g_projection);
	cube.SetEyeRbt(&g_eye_rbt);

	mat4 cube_rbt = translate(g_world_rbt, vec3(3.0f, -3.0f, 0.0f));
	g_model_rbts.push_back(cube_rbt);
	g_model_rotation_speed.push_back(0);
	cube.SetModelRbt(&g_model_rbts[model_count++]);
	cube.SetDirectionalLight(dir_light);

	g_models.push_back(cube);
	
	Model another_cube = Model();
	InitDataCube(another_cube, vec3(0.03f, 0.03f, 0.03f)); // Black tilted control console
	another_cube.InitializeGLSL(DRAW_TYPE::ARRAY, "VertexShader.glsl", "FragmentShader.glsl");
	another_cube.SetProjection(&g_projection);
	another_cube.SetEyeRbt(&g_eye_rbt);

	mat4 another_cube_rbt = translate(g_world_rbt, vec3(-1.5f, -2.0f, 0.0f)) * rotate(mat4(1.0f), 75.0f, normalize(vec3(0.4f, 0.8f, 0.6f)));
	g_model_rbts.push_back(another_cube_rbt);
	g_model_rotation_speed.push_back(0);
	another_cube.SetModelRbt(&g_model_rbts[model_count++]);
	another_cube.SetDirectionalLight(dir_light);

	g_models.push_back(another_cube);

	// 2. test another model

	Model knot = Model();
	InitDataKnot(knot, vec3(0.5f, 0.5f, 0.0f)); // An hourglass
	knot.InitializeGLSL(DRAW_TYPE::ARRAY, "VertexShader.glsl", "FragmentShader.glsl");
	knot.SetProjection(&g_projection);
	knot.SetEyeRbt(&g_eye_rbt);

	mat4 knot_rbt = translate(g_world_rbt, vec3(1.5f, -2.0f, 0.0f)) * rotate(mat4(1.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), 90.0f, vec3(0.0f, 1.0f, 0.0f));
	g_model_rbts.push_back(knot_rbt);
	g_model_rotation_speed.push_back(0);
	knot.SetModelRbt(&g_model_rbts[model_count++]);
	knot.SetDirectionalLight(dir_light);

	g_models.push_back(knot);

	Model another_knot = Model();
	InitDataKnot(another_knot, vec3(0.408f, 0.129f, 0.478f)); // The color of the logo from "Visual Studio 2015"
	another_knot.InitializeGLSL(DRAW_TYPE::ARRAY, "VertexShader.glsl", "FragmentShader.glsl");
	another_knot.SetProjection(&g_projection);
	another_knot.SetEyeRbt(&g_eye_rbt);

	mat4 another_knot_rbt = translate(g_world_rbt, vec3(-1.5f, -2.0f, 0.0f)) * rotate(mat4(1.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
	g_model_rbts.push_back(another_knot_rbt);
	g_model_rotation_speed.push_back(0);
	another_knot.SetModelRbt(&g_model_rbts[model_count++]);
	another_knot.SetDirectionalLight(dir_light);

	g_models.push_back(another_knot);

	/* 4. Implement mobile using binary tree implementation.
	Since I finished placing them, I can access childs from g_models[i] using g_models[2*i+1] and g_models[2*i+2].
	By accessing them, I can compute the distance before the rotation starts.
	Traversal for rotation: BFS (pressing 'o' increases the index by 1).
	Traversal for applying rotation: DFS
	*/

	Model another_pyramid = Model();
	InitDataPyramid(another_pyramid, vec3(0.0f, 0.750f, 1.0f));  // The color of the guide stone from "The Legend of Zelda: Breath of the Wild"
	another_pyramid.InitializeGLSL(DRAW_TYPE::ARRAY, "VertexShader.glsl", "FragmentShader.glsl");
	another_pyramid.SetProjection(&g_projection);
	another_pyramid.SetEyeRbt(&g_eye_rbt);

	mat4 another_pyramid_rbt = translate(g_world_rbt, vec3(1.5f, -2.0f, 0.0f)) * rotate(mat4(1.0f), 180.0f, vec3(1.0f, 0.0f, 0.0f));
	g_model_rbts.push_back(another_pyramid_rbt);
	g_model_rotation_speed.push_back(0);
	another_pyramid.SetModelRbt(&g_model_rbts[model_count++]);
	another_pyramid.SetDirectionalLight(dir_light);

	g_models.push_back(another_pyramid);

	lastTime = (float)glfwGetTime();
	do{
		// Clear the screen	
		// Fill the background
		glClearColor((GLclampf)0.0f, (GLclampf)0.0f, (GLclampf)0.0f, (GLclampf)0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		currentTime = (float)glfwGetTime();
		g_world_rbt = g_world_rbt * rotate(mat4(1.0f), (currentTime - lastTime)*g_world_rbt_rotation_speed_x, vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), (currentTime - lastTime)*g_world_rbt_rotation_speed_z, vec3(0.0f, 0.0f, 1.0f)); // the world spins

		model_view = g_world_rbt;
		
		traverse(0);

		// Swap buffers
		glfwSwapBuffers(g_window);
		lastTime = currentTime;
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(g_window) == 0);

	// Cleanup all resources
	CleanUp();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
