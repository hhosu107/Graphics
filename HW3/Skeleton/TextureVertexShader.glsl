#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertex_uv;

// Output data ; will be interpolated for each fragment.
out vec3 fragmentPosition;
out vec3 fragmentNormal;
out vec2 fragmentUV;
out vec3 fragmentWorldPosition;
out vec3 fragmentWorldNormal;

uniform mat4 ModelTransform;
uniform mat4 Eye;
uniform mat4 Projection;

mat4 NormalMatrix(mat4 MVM)
{
	mat4 invm = inverse(MVM);
	invm[0][3] = 0;
	invm[1][3] = 0;
	invm[2][3] = 0;

	return transpose(invm);
}

void main(){	

	// Output position of the vertex, in world space: Model * position
	fragmentWorldPosition = (ModelTransform * vec4(vertexPosition_modelspace, 1.0)).xyz;
	fragmentWorldNormal = (NormalMatrix(ModelTransform) * vec4(vertexNormal_modelspace, 0.0)).xyz;

	// Output position of the vertex, in clip space : MVP * position
	mat4 MVM = inverse(Eye) * ModelTransform;
	mat4 NVM = NormalMatrix(MVM);

	vec4 wPosition = MVM * vec4(vertexPosition_modelspace,1);
	fragmentPosition = wPosition.xyz;
	gl_Position = Projection * wPosition;
	fragmentNormal = (NVM * vec4(vertexNormal_modelspace,1.0)).xyz;

	// The color of each vertex will be interpolated
	// to produce the color of each fragment
	fragmentUV = vertex_uv;
}

