#version 330 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;
// Interpolated values from the vertex shaders
in vec2 fragmentUV;
in vec3 fragmentWorldPosition;
in vec3 fragmentWorldNormal;

// colormodel material has a constant diffuse and specular intensity and shininess.
struct Material
{
	float alpha;
};

// directional light
struct Directional
{
	vec3 dir; // direction of directional light
	vec3 color;
};

// point light
struct Point
{
	vec3 pos; // position of point light
	vec3 color;

	// attenuation term by distance
	float a; 
	float b;
	float c;
};

// spot light

struct Spot
{
	vec3 pos;
	vec3 color;

	vec3 dir;
	float cutoff; // intensity goes to 0 after exceeding cutoff angle

	// attenuation term by distance
	float a;
	float b;
	float c;
};

uniform Material material;
uniform Directional directional[3];
uniform Point point[2];
uniform Spot spot;

uniform float d; // diffuse term
uniform float s; // specular term

uniform mat4 ModelTransform;
uniform mat4 Eye; // location of the camera
uniform mat4 Projection;

uniform int light[6];
uniform int blinn;

// Ouput data
out vec3 color;

uniform sampler2D tex;

mat4 NormalMatrix(mat4 MVM)
{
	mat4 invm = inverse(MVM);
	invm[0][3] = 0;
	invm[1][3] = 0;
	invm[2][3] = 0;

	return transpose(invm);
}

vec3 pointLight(Point light, vec3 normal, vec3 viewDirection, vec3 textureColor){
	// imagine that the eye is located at the fragment and see the point light directly (world space)
	vec3 lightDirection = normalize(light.pos - fragmentWorldPosition);
	float spec;
	float dist = length(light.pos - fragmentWorldPosition); // distance on the world space
	float attenuation = 1.0f / (light.a + light.b * dist + light.c * dist * dist); // attenuation factor

	// HW3: Gooch Shading
	float diff = (1.0 + dot(normal, lightDirection))/2;
	
	if (diff >= 0.95){
		diff = 1.0;
	}
	else if(diff >= 0.86){
		diff = 0.75;
	}
	else if(diff >= 0.80){
		diff = 0.5;
	}
	else if(diff >= 0.72){
		diff = 0.25;
	}
	else diff = 0.05;
	
	// HW3: Gooch shading
	// cool color mixed with color of the object
	// vec3 coolColorMod = coolColor + fragmentColor * alpha;
	vec3 coolColorMod = vec3(0.0, 0.5, 0.0) + textureColor * 0.5;
    // warm color mixed with color of the object
	// vec3 warmColorMod = warmColor + fragmentColor * beta;
    vec3 warmColorMod = vec3(0.4, 0.0, 0.7) + textureColor * 0.3;
    // interpolation of cool and warm colors according 
    // to lighting intensity. The lower the light intensity,
    // the larger part of the cool color is used
    return attenuation * mix(coolColorMod, warmColorMod, diff);
}

void main(){

	// fragment normal on world space
	vec3 normal = normalize(fragmentWorldNormal);
	// fragment view direction on world space: first represent the eye on the world coordinate and then compute the view vector
	vec3 viewDirection = -normalize((inverse(Eye) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - fragmentWorldPosition);
	// color by directional light
	vec3 intensity = vec3(0.01, 0.01, 0.01); // ambient term initialization
	
	// HW3: Gooch Shading
	vec3 textureColor = texture(tex, fragmentUV).xyz; // texture's real color
	intensity = pointLight(point[1], normal, viewDirection, textureColor);
	color = intensity;
}