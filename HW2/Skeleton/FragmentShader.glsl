#version 330 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;
// Interpolated values from the vertex shaders
in vec3 fragmentColor;
in vec3 fragmentWorldPosition;
in vec3 fragmentWorldNormal;
// Light location
/* uniform vec3 lightPosition; */

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
	//vec3 diff; // diffuse constant vector
	//vec3 spec; // specular constant vector
};

// point light
struct Point
{
	vec3 pos; // position of point light
	vec3 color;
	//vec3 diff;
	//vec3 spec;

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
	//vec3 diff;
	//vec3 spec;

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

uniform float d;
uniform float s;

uniform mat4 ModelTransform;
uniform mat4 Eye; // location of the camera
uniform mat4 Projection;

uniform int light[6];

// Ouput data
out vec3 color;

mat4 NormalMatrix(mat4 MVM)
{
	mat4 invm = inverse(MVM);
	invm[0][3] = 0;
	invm[1][3] = 0;
	invm[2][3] = 0;

	return transpose(invm);
}

// Assume that the constant for the ambient light is equal to the constant for the diffuse light.
vec3 directionalLight(Directional light, vec3 normal, vec3 viewDirection){

	vec3 lightDirection = normalize(light.dir); // light vector
	//vec3 reflection = reflect(-lightDirection, normal); // according to phong model
	
	float diff = max(dot(normal, lightDirection), 0.0); // = \bar{cosine}(angle between normal and light)

	vec3 halfVec = normalize(lightDirection + viewDirection);
	float spec = pow(max(dot(normal, halfVec), 0.0), material.alpha);
	//float spec = pow(max(dot(viewDirection, reflection), 0.0), material.alpha); // = (reflection \cdot viewDirection)^{shininess}

	vec3 diffuse, specular;
	diffuse = d * diff * light.color;
	specular = s * spec * light.color;
	//diffuse = light.diff * diff;
	//specular = light.spec * spec;
	return (diffuse + specular);
}

vec3 pointLight(Point light, vec3 normal, vec3 viewDirection){
	vec3 lightDirection = normalize(light.pos - fragmentWorldPosition); // imagine that the eye is located at the fragment and see the point light directly (world space)
	//vec3 reflection = reflect(-lightDirection, normal); // according to phong model
	float dist = length(light.pos - fragmentWorldPosition); // distance on the world space
	float attenuation = 1.0f / (light.a + light.b * dist + light.c * dist * dist); // attenuation factor

	float diff = max(dot(normal, lightDirection), 0.0);
	
	vec3 halfVec = normalize(lightDirection + viewDirection);
	float spec = pow(max(dot(normal, halfVec), 0.0), material.alpha);
	
	//float spec = pow(max(dot(viewDirection, reflection), 0.0), material.alpha); // = (reflection \cdot viewDirection)^{shininess}

	vec3 diffuse, specular;
	diffuse = d * diff * light.color;
	specular = s * spec * light.color;
	//diffuse = light.diff * diff;
	//specular = light.spec * spec;
	return (diffuse * attenuation + specular * attenuation);
}

vec3 spotLight(Spot light, vec3 normal, vec3 viewDirection){
	vec3 lightDirection = normalize(light.pos - fragmentWorldPosition); // imagine that the eye is located at the fragment and see the point light directly (world space) 
	//vec3 reflection = reflect(-lightDirection, normal); // according to phong model
	float dist = length(light.pos - fragmentWorldPosition);  // distance on the world space
	float attenuation = 1.0f / (light.a + light.b * dist + light.c * dist * dist); // attenuation factor

	float diff = max(dot(normal, lightDirection), 0.0);

	vec3 halfVec = normalize(lightDirection + viewDirection);
	float spec = pow(max(dot(normal, halfVec), 0.0), material.alpha);
	
	//float spec = pow(max(dot(viewDirection, reflection), 0.0), material.alpha); // = (reflection \cdot viewDirection)^{alpha}

	vec3 diffuse, specular;
	diffuse = d * diff * light.color;
	specular = s * spec * light.color;
	//diffuse = light.diff * diff;
	//specular = light.spec * spec;
	return (diffuse * attenuation + specular * attenuation);
}

void main(){

	// fragment normal on world space
	vec3 normal = normalize(fragmentWorldNormal);
	// fragment view direction on world space
	vec3 viewDirection = normalize((inverse(Eye) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - fragmentWorldPosition); // direction from eye to the fragment
	// color by directional light
	vec3 intensity = vec3(0.1, 0.1, 0.1); // ambient term initialization
	for(int i=0; i<3; i++){
		intensity = intensity + (light[i]) * directionalLight(directional[i], normal, viewDirection);
	}
	// vec3 result = directionalLight(directional, normal, viewDirection);
	// color by point light
	for(int i=0; i<2; i++){
		intensity = intensity + (light[3+i]) * pointLight(point[i], normal, viewDirection);
	}
	// color by spot light
	vec3 spotDirection = normalize(spot.pos - fragmentWorldPosition);
	if(acos(dot(-spotDirection, normalize(spot.dir))) < spot.cutoff){
		intensity = intensity + (light[5]) * spotLight(spot, normal, viewDirection);
	}

	color = pow(fragmentColor * intensity, vec3(1.0/2.2)); // Apply gamma correction

	// float diffuse = max(0.0, dot(normal, tolight));
	// vec3 intensity = fragmentColor * diffuse;

	// color = pow(intensity, vec3(1.0/2.2)); // Apply gamma correction
}


