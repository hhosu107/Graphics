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

// Include common libraries
#include "node.hpp"
#include "common/rtt.hpp"
#include <common/shader.hpp>

// Include SOIL static library
#include "SOIL/SOIL.h"

using namespace std;
using namespace glm;

enum GEOMETRY_TYPE {
	CUBE,
	SPHERE,
	CYLINDER,
	PYRAMID
};

// Global variable
GLFWwindow* g_window;
float g_window_width = 1024.f;
float g_window_height = 768.f;
int g_framebuffer_width = 1024;
int g_framebuffer_height = 768;
float g_fov = 45.f; // 45-degree of fov
float g_fovy = g_fov; // y-angle of fov

int g_nodes_index = 0;
int g_lights_index = 0;

// Node and Models
vector<Node*> g_nodes;

// Transformations
mat4 g_projection;
// Camera model property
mat4 g_eye_rbt;
mat4 fixed_eye_rbt;
// World model property
mat4 g_world_rbt = mat4(1.0f);

// Render To Texture
RTT render_to_texture;
// Render surround walls and floor/ceiling
RTT surround_rtt[6];

// SOIL texture loading
/*
GLuint tex_cube = SOIL_load_OGL_cubemap
(
	"./resources/images/skygnd.bmp",
	"./resources/images/skygnd.bmp",
	"./resources/images/skygnd.bmp",
	"./resources/images/skygnd.bmp",
	"./resources/images/sky.bmp",
	"./resources/images/gnd.bmp",
	SOIL_LOAD_AUTO,
	SOIL_CREATE_NEW_ID,
	SOIL_FLAG_MIPMAPS
);
*/
// Sample from TA
/*
int width, height;
unsigned char* image = SOIL_load_image("img.png", &width, &height, 0, SOIL_LOAD_RGB);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
SOIL_free_image_data(image);
*/

// Event functions
static void WindowSizeCallback(GLFWwindow*, int, int);
static void MouseButtonCallback(GLFWwindow*, int, int, int);
static void CursorPosCallback(GLFWwindow*, double, double);
static void KeyboardCallback(GLFWwindow*, int, int, int, int);
void UpdateFovy(void);
void CleanUp(void);
void AddColorModel(ColorModel& a_model, GLuint a_program, GEOMETRY_TYPE a_type, vec3 a_color);

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
	for (int i = 0; i < g_nodes.size(); i++)
	{
		g_nodes[i]->CleanUp();
	}
}

void AddColorModel(ColorModel& a_model, GLuint a_program, GEOMETRY_TYPE a_type, vec3 a_color)
{
	/*
	* Setting a model
	* 1. Initialize data for model;
	* 2. Transfer to vertex buffer
	* 3. Set projection, eye matrix, model matrix (in this case we will cover in node class), glsl program
	*/
	switch (a_type)
	{
	case CUBE:
		InitDataCube(a_model, a_color);
		a_model.InitializeGLSL(ARRAY);
		break;
	case SPHERE:
		InitDataSphere(a_model, a_color);
		a_model.InitializeGLSL(INDEX);
		break;
	case CYLINDER:
		InitDataCylinder(a_model, a_color);
		a_model.InitializeGLSL(ARRAY);
		break;
	case PYRAMID:
		InitDataPyramid(a_model, a_color);
		a_model.InitializeGLSL(ARRAY);
		break;
	}

	a_model.SetProjection(&g_projection);
	a_model.SetEyeRbt(&g_eye_rbt);
	a_model.SetProgram(a_program);
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

	// Update RTT
	render_to_texture.CreateTexture(g_framebuffer_width, g_framebuffer_height);
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
			cout << "p\t\t Enable/Disable picking" << endl;
			break;
		case GLFW_KEY_O:
			g_nodes_index += 1;
			g_nodes_index = g_nodes_index % g_nodes.size();
			break;
		case GLFW_KEY_M:
			g_nodes[g_nodes_index]->AddSpeed(10.0f);
			break;
		case GLFW_KEY_N:
			g_nodes[g_nodes_index]->AddSpeed(-10.0f);
			break;
		case GLFW_KEY_R:
			g_nodes[g_nodes_index]->ResetSpeed();
			break;
		case GLFW_KEY_UP:
			g_eye_rbt = translate(vec3(0.0f, 1.0f, 0.0f)) * g_eye_rbt;
			//fixed_eye_rbt = translate(vec3(0.0f, 1.0f, 0.0f)) * fixed_eye_rbt;
			break;
		case GLFW_KEY_DOWN:
			g_eye_rbt = translate(vec3(0.0f, -1.0f, 0.0f)) * g_eye_rbt;
			//fixed_eye_rbt = translate(vec3(0.0f, -1.0f, 0.0f)) * fixed_eye_rbt;
			break;
		case GLFW_KEY_LEFT:
			g_eye_rbt = translate(vec3(-1.0f, 0.0f, 0.0f)) * g_eye_rbt;
			//fixed_eye_rbt = translate(vec3(-1.0f, 0.0f, 0.0f)) * fixed_eye_rbt;
			break;
		case GLFW_KEY_RIGHT:
			g_eye_rbt = translate(vec3(1.0f, 0.0f, 0.0f)) * g_eye_rbt;
			//fixed_eye_rbt = translate(vec3(1.0f, 0.0f, 0.0f)) * fixed_eye_rbt;
			break;
		case GLFW_KEY_LEFT_BRACKET:
			g_eye_rbt = translate(vec3(0.0f, 0.0f, -1.0f)) * g_eye_rbt;
			//fixed_eye_rbt = translate(vec3(0.0f, 0.0f, -1.0f)) * fixed_eye_rbt;
			break;
		case GLFW_KEY_RIGHT_BRACKET:
			g_eye_rbt = translate(vec3(0.0f, 0.0f, 1.0f)) * g_eye_rbt;
			//fixed_eye_rbt = translate(vec3(0.0f, 0.0f, 1.0f)) * fixed_eye_rbt;
			break;
		default:
			break;
		}
	}
}


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Open a window and create its OpenGL context
	g_window = glfwCreateWindow(1024, 768, "Homework 2: 20173245 - Chansu Park", NULL, NULL);

	if (g_window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
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

	g_eye_rbt = translate(mat4(1.0f), vec3(0.0, 0.0f, 10.0));
	fixed_eye_rbt = translate(mat4(1.0f), vec3(0.0, -2.0f, 10.0));

	// Initialize shader program (for efficiency)
	GLuint diffuse_shader = LoadShaders("VertexShader.glsl", "FragmentShader.glsl");
	GLuint line_shader = LoadShaders("LineVertexShader.glsl", "LineFragmentShader.glsl");
	GLuint texture_shader = LoadShaders("TextureVertexShader.glsl", "TextureFragmentShader.glsl");

	/*
	* Node configuration
	* 1. Setting node
	* 2. Update hierarchical frame and setting lines for them
	* 3. Add models to each node
	* 4. Update object frame
	*/

	// 1
	Node root = Node();
	Node f_1 = Node();
	Node f_2 = Node();
	Node f_3 = Node();
	Node f_11 = Node();
	Node f_12 = Node();

	g_nodes.push_back(&root); g_nodes.push_back(&f_1); g_nodes.push_back(&f_2); g_nodes.push_back(&f_3);
	g_nodes.push_back(&f_11); g_nodes.push_back(&f_12);

	root.AddChild(&f_1); root.AddChild(&f_2); root.AddChild(&f_3);
	f_1.AddChild(&f_11); f_1.AddChild(&f_12);

	// 2
	root.InitialChildrenFrame();
	for (int i = 0; i < g_nodes.size(); i++)
	{
		g_nodes[i]->SetLines(line_shader, &g_projection, &g_eye_rbt);
	}

	// 3.
	ColorModel r_m1 = ColorModel();
	AddColorModel(r_m1, diffuse_shader, CUBE, vec3(1.0f, 1.0f, 0.0f));
	root.AddModel(&r_m1, mat4(1.0f));

	// Multiple objects for same basis
	ColorModel f1_m1 = ColorModel();
	AddColorModel(f1_m1, diffuse_shader, PYRAMID, vec3(1.0f, 0.0f, 0.0f));
	f_1.AddModel(&f1_m1, translate(mat4(1.0f), vec3(0.0f, 0.5f, 0.0f)));

	ColorModel f1_m2 = ColorModel();
	AddColorModel(f1_m2, diffuse_shader, PYRAMID, vec3(0.0f, 1.0f, 0.0f));
	f_1.AddModel(&f1_m2, translate(mat4(1.0f), vec3(-0.5f, 0.0f, 0.0f)));

	ColorModel f1_m3 = ColorModel();
	AddColorModel(f1_m3, diffuse_shader, PYRAMID, vec3(0.0f, 0.0f, 1.0f));
	f_1.AddModel(&f1_m3, translate(mat4(1.0f), vec3(0.5f, 0.0f, 0.0f)));

	ColorModel f2_m1 = ColorModel();
	AddColorModel(f2_m1, diffuse_shader, SPHERE, vec3(0.0f, 1.0f, 1.0f));
	f_2.AddModel(&f2_m1, mat4(1.0f) * scale(vec3(3.0f, 1.0f, 2.0f)));

	ColorModel f3_m1 = ColorModel();
	AddColorModel(f3_m1, diffuse_shader, CYLINDER, vec3(0.0f, 0.0f, 1.0f));
	f_3.AddModel(&f3_m1, mat4(1.0f));

	ColorModel f11_m1 = ColorModel();
	AddColorModel(f11_m1, diffuse_shader, CUBE, vec3(1.0f, 1.0f, 0.0f));
	f_11.AddModel(&f11_m1, mat4(1.0f));

	ColorModel f12_m1 = ColorModel();
	AddColorModel(f12_m1, diffuse_shader, CUBE, vec3(1.0f, 0.0f, 1.0f));
	f_12.AddModel(&f12_m1, mat4(1.0f));

	// 4.
	root.UpdateObjectFrame();

	TextureModel test_model = TextureModel();
	mat4 test_m = scale(vec3(10.0, 10.0, 10.0));
	// InitDataCube(test_model);
	InitDataCubeInternal(test_model);
	test_model.InitializeGLSL(ARRAY);
	test_model.SetProjection(&g_projection);
	test_model.SetEyeRbt(&g_eye_rbt);
	// surround_m[0] = translate(vec3(-test_m[2]) / 2) * rotate(mat4(1.0f), 0.0f, vec3(0.0, 1.0, 0.0)) * test_m;
	test_model.SetModelRbt(&test_m);
	test_model.SetProgram(texture_shader);
	render_to_texture = RTT();
	render_to_texture.CreateTexture(g_framebuffer_width, g_framebuffer_height);

	TextureModel surround[6];
	mat4 surround_m[6];
	// Walls
	// back
	surround[0] = TextureModel();
	InitDataSquare(surround[0]);
	surround[0].InitializeGLSL(ARRAY);
	surround[0].SetProjection(&g_projection);
	// surround[0].SetEyeRbt(&fixed_eye_rbt);
	surround[0].SetEyeRbt(&g_eye_rbt);
	surround_m[0] = translate(vec3(-test_m[2]) / 2) * rotate(mat4(1.0f), 0.0f, vec3(0.0, 1.0, 0.0)) * test_m;
	surround[0].SetModelRbt(&surround_m[0]);
	surround[0].SetProgram(texture_shader);
	surround_rtt[0] = RTT();
	surround_rtt[0].SetTexture("../Resources/images/skygnd.png");

	// left
	surround[1] = TextureModel();
	InitDataSquare(surround[1]);
	surround[1].InitializeGLSL(ARRAY);
	surround[1].SetProjection(&g_projection);
	surround[1].SetEyeRbt(&g_eye_rbt);
	//surround[1].SetEyeRbt(&fixed_eye_rbt);
	surround_m[1] = translate(vec3(-test_m[0]) / 2) * rotate(mat4(1.0f), 2.0f*atanf(1.0f), vec3(0.0, 1.0, 0.0)) * test_m;
	surround[1].SetModelRbt(&surround_m[1]);
	surround[1].SetProgram(texture_shader);
	surround_rtt[1] = RTT();
	surround_rtt[1].SetTexture("../Resources/images/skygnd.png");

	// forth
	surround[2] = TextureModel();
	InitDataSquare(surround[2]);
	surround[2].InitializeGLSL(ARRAY);
	surround[2].SetProjection(&g_projection);
	surround[2].SetEyeRbt(&g_eye_rbt);
	//surround[2].SetEyeRbt(&fixed_eye_rbt);
	surround_m[2] = translate(vec3(test_m[2]) / 2) * rotate(mat4(1.0f), 4.0f*atanf(1.0f), vec3(0.0, 1.0, 0.0)) * test_m;
	surround[2].SetModelRbt(&surround_m[2]);
	surround[2].SetProgram(texture_shader);
	surround_rtt[2] = RTT();
	surround_rtt[2].SetTexture("../Resources/images/skygnd.png");

	// right
	surround[3] = TextureModel();
	InitDataSquare(surround[3]);
	surround[3].InitializeGLSL(ARRAY);
	surround[3].SetProjection(&g_projection);
	surround[3].SetEyeRbt(&g_eye_rbt);
	//surround[3].SetEyeRbt(&fixed_eye_rbt);
	surround_m[3] = translate(vec3(test_m[0]) / 2) * rotate(mat4(1.0f), 6.0f*atanf(1.0f), vec3(0.0, 1.0, 0.0)) * test_m;
	surround[3].SetModelRbt(&surround_m[3]);
	surround[3].SetProgram(texture_shader);
	surround_rtt[3] = RTT();
	surround_rtt[3].SetTexture("../Resources/images/skygnd.png");

	// Ceiling
	surround[4] = TextureModel();
	InitDataSquare(surround[4]);
	surround[4].InitializeGLSL(ARRAY);
	surround[4].SetProjection(&g_projection);
	surround[4].SetEyeRbt(&g_eye_rbt);
	//surround[4].SetEyeRbt(&fixed_eye_rbt);
	surround_m[4] = translate(vec3(test_m[1]) / 2) * rotate(mat4(1.0f), 2.0f*atanf(1.0f), vec3(1.0, 0.0, 0.0)) * test_m;
	surround[4].SetModelRbt(&surround_m[4]);
	surround[4].SetProgram(texture_shader);
	surround_rtt[4] = RTT();
	surround_rtt[4].SetTexture("../Resources/images/gnd.png");

	// Floor
	surround[5] = TextureModel();
	InitDataSquare(surround[5]);
	surround[5].InitializeGLSL(ARRAY);
	surround[5].SetProjection(&g_projection);
	surround[5].SetEyeRbt(&g_eye_rbt);
	//surround[5].SetEyeRbt(&fixed_eye_rbt);
	surround_m[5] = translate(-vec3(test_m[1]) / 2) * rotate(mat4(1.0f), -2.0f*atanf(1.0f), vec3(1.0, 0.0, 0.0)) * test_m;
	surround[5].SetModelRbt(&surround_m[5]);
	surround[5].SetProgram(texture_shader);
	surround_rtt[5] = RTT();
	surround_rtt[5].SetTexture("../Resources/images/sky.png");

	// now we have to set texture for each texture model.


	double prev_time = glfwGetTime();

	do {
		// Clear the screen	
		// Fill the background

		double current_time = glfwGetTime();
		double elapsed_time = current_time - prev_time;

		prev_time = current_time;

		// Scene update
		for (int i = 0; i < g_nodes.size(); i++)
		{
			g_nodes[i]->ApplyRotation((float)elapsed_time);
		}

		//render_to_texture.BindFBO();
		glClearColor((GLclampf)1.0f, (GLclampf)1.0f, (GLclampf)1.0f, (GLclampf)1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 5. Draw
		for (int i = 0; i < g_nodes.size(); i++)
		{
			if (i == g_nodes_index)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				g_nodes[i]->Draw();
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else
			{
				g_nodes[i]->Draw();
			}
		}
		//render_to_texture.UnbindFBO();

		// test_m = test_m * rotate(mat4(1.0f), (float) (0.3 * elapsed_time), vec3(1.0, 1.0, 1.0));

		/*
		glClearColor((GLclampf)1.0f, (GLclampf)1.0f, (GLclampf)1.0f, (GLclampf)1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		test_model.SetColorTexture(render_to_texture.GetTexture());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		test_model.Draw();
		*/
		/*
		glClearColor((GLclampf)1.0f, (GLclampf)1.0f, (GLclampf)1.0f, (GLclampf)1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		*/
		for (int i = 0; i < 6; i++) {
			surround[i].SetColorTexture(surround_rtt[i].GetTexture());
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			surround[i].Draw();
		}

		// Swap buffers
		glfwSwapBuffers(g_window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(g_window) == 0);

	// Cleanup all resources
	CleanUp();

	// Clean up shader
	glDeleteProgram(diffuse_shader);
	glDeleteProgram(line_shader);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
