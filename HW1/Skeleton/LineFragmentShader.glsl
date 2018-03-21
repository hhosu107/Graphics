#version 330 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;
// Interpolated values from the vertex shaders
in vec3 fragmentColor;

// Ouput data
out vec3 color;

uniform vec3 uLight;

void main(){
	

	color = vec3(1.0); // Apply gamma correction
}