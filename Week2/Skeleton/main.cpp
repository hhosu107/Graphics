// Include standard headers
#include <iostream>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <common/shader.hpp>

using namespace std;
using namespace glm;

// Global variable
GLFWwindow* window;

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
	window = glfwCreateWindow( 1024, 768, "OpenGL Tutorial: STUDENT_ID", NULL, NULL);

	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window." );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Enable back-face culling
	glEnable(GL_CULL_FACE);

	// Setting viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// 1. Prepare to draw

	std::vector<vec3> g_vertex_buffer_data = std::vector<vec3>();
	std::vector<vec3> g_color_buffer_data = std::vector<vec3>();
	// Now assign indices
	std::vector<unsigned short> indices = std::vector<unsigned short>();

	// Now make 8 vertices
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, -1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, 1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, 1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, 1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, -1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, -1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, -1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, 1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, 1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, 1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, -1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, -1.0f, 1.0f));
	
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, -1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, 1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, 1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, 1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, -1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, -1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, -1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, 1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, 1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, 1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, -1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, -1.0f, -1.0f));

	g_vertex_buffer_data.emplace_back(vec3(-1.0f, 1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, 1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, 1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, 1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, 1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, 1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, -1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, -1.0f, -1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, -1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(1.0f, -1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, -1.0f, 1.0f));
	g_vertex_buffer_data.emplace_back(vec3(-1.0f, -1.0f, -1.0f));

	g_color_buffer_data.emplace_back(vec3(0.0f, 0.0f, 0.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 1.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(1.0f, 0.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(1.0f, 0.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(1.0f, 1.0f, 0.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 0.0f, 0.0f));

	g_color_buffer_data.emplace_back(vec3(1.0f, 0.0f, 0.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 1.0f, 0.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 0.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 0.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(1.0f, 1.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(1.0f, 0.0f, 0.0f));

	g_color_buffer_data.emplace_back(vec3(0.0f, 0.0f, 0.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 1.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 1.0f, 0.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 1.0f, 0.0f));
	g_color_buffer_data.emplace_back(vec3(1.0f, 0.0f, 0.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 0.0f, 0.0f));

	g_color_buffer_data.emplace_back(vec3(1.0f, 1.0f, 0.0f));
	g_color_buffer_data.emplace_back(vec3(1.0f, 0.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 0.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 0.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(1.0f, 1.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(1.0f, 1.0f, 0.0f));

	g_color_buffer_data.emplace_back(vec3(0.0f, 1.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(1.0f, 0.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 0.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 0.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 1.0f, 0.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 1.0f, 1.0f));

	g_color_buffer_data.emplace_back(vec3(0.0f, 0.0f, 0.0f));
	g_color_buffer_data.emplace_back(vec3(1.0f, 1.0f, 0.0f));
	g_color_buffer_data.emplace_back(vec3(1.0f, 1.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(1.0f, 1.0f, 1.0f));
	g_color_buffer_data.emplace_back(vec3(1.0f, 0.0f, 0.0f));
	g_color_buffer_data.emplace_back(vec3(0.0f, 0.0f, 0.0f));

	// Now implace indices (12 * 3 times for cube)
	/* indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(2);
	indices.emplace_back(3);
	indices.emplace_back(0);
	indices.emplace_back(4);
	indices.emplace_back(5);
	indices.emplace_back(6);
	indices.emplace_back(6);
	indices.emplace_back(7);
	indices.emplace_back(4);
	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(5);
	indices.emplace_back(5);
	indices.emplace_back(4);
	indices.emplace_back(0);
	indices.emplace_back(3);
	indices.emplace_back(2);
	indices.emplace_back(6);
	indices.emplace_back(6);
	indices.emplace_back(7);
	indices.emplace_back(3);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(6);
	indices.emplace_back(6);
	indices.emplace_back(5);
	indices.emplace_back(1);
	indices.emplace_back(0);
	indices.emplace_back(3);
	indices.emplace_back(7);
	indices.emplace_back(7);
	indices.emplace_back(4);
	indices.emplace_back(0);*/
	// 2. Transform
	// Model Matrix: an identity matrix (model will be at the origin)
	mat4 Model = mat4(1.0f);
	// Camera Matrix:
	mat4 View = lookAt(vec3(-3.0f, -3.0f, -3.0f),
		vec3(0.0f, 0.0f, 0.0f), 
		vec3(0.0f, 1.0f, 0.0f));
	// Projection Matrix: 45-degree field of view, 4:3 ratio, display range: 0.1 unit <--> 100 units
	mat4 Projection = perspective(90.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	// 2-1. prepare to change model matrix
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	// 3. How To Transfer Our Data To GPU?
	
	// Initialize the shader
	GLuint programID = LoadShaders("TransformVertexShader.glsl", "ColorFragmentShader.glsl");

	// 3.1 Vertex data
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size() * sizeof(vec3), &g_vertex_buffer_data[0], GL_STATIC_DRAW);

	GLuint colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, g_color_buffer_data.size() * sizeof(vec3), &g_color_buffer_data[0], GL_STATIC_DRAW);

	// 3.2 Uniform data	
	GLuint modelMatrixID = glGetUniformLocation(programID, "Model");
	GLuint viewMatrixID = glGetUniformLocation(programID, "View");
	GLuint projectionMatrixID = glGetUniformLocation(programID, "Projection");

	do{
		// Clear the screen
		double currentTime = glfwGetTime();
		nbFrames++;
		Model = mat4(vec4(cosf((float)nbFrames / 1000), sinf((float)nbFrames / 1000), 0, 0),
			vec4(-sinf((float)nbFrames / 1000), cosf((float)nbFrames / 1000), 0, 0),
			vec4(0,0, 1, 0),
			vec4(0,0,0,1));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 3. How To Transfer Our Data To GPU?
		// 3.1 Pass vertex data
		// Specify shader program
		glUseProgram(programID);

		// Bind vertex array
		glBindVertexArray(VertexArrayID);
		// 1st attribute buffer: position
		// Bind buffer data
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		// Specify attribute for buffer data
		glVertexAttribPointer(
			0,					// attribute
			3,					// size
			GL_FLOAT,			// type
			GL_FALSE,			// normalized?
			0,					// stride
			(void*)0			// array buffer offset
		);
		glEnableVertexAttribArray(0);

		// 2nd attribute buffer: color
		// Bind buffer data
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		// Specify attribute for buffer data
		glVertexAttribPointer(
			1,					// attribute
			3,					// size
			GL_FLOAT,			// type
			GL_FALSE,			// normalized?
			0,					// stride
			(void*)0			// array buffer offset
		);
		glEnableVertexAttribArray(1);
		// 3.2 Pass uniform data
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &Model[0][0]);
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &Projection[0][0]);

		// 3.3 Draw triangles!
		// Instead of glDrawArrays,
		glDrawArrays(GL_TRIANGLES, 0, g_vertex_buffer_data.size());
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );
	
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
