#version 330 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;
// Interpolated values from the vertex shaders
in vec2 fragmentUV;

// Ouput data
out vec3 color;

uniform mat4 ModelTransform;
uniform mat4 Eye;
uniform mat4 Projection;

uniform sampler2D tex;

void main(){
	vec3 intensity = texture(tex, fragmentUV).xyz;
	
	color = pow(intensity, vec3(1.0/2.2)); // Apply gamma correction
}