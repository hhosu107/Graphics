#version 330 core

// Ouput data
out vec3 color;

void main(){
	color = pow(vec3(1.0f,1.0f,1.0f), vec3(1.0/2.2)); // Apply gamma correction
}