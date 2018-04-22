#version 330 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;
// Interpolated values from the vertex shaders
in vec3 fragmentColor;
// Light location
/* uniform vec3 lightPosition; */

// colormodel material has a constant diffuse and specular intensity and shininess.
struct Material
{
	float diffuseIntensity;
	float specularIntensity;
	float shininess;
};

// directional light
struct Directional
{
	vec3 dir; // direction of directional light
	vec3 amb; // ambient constant vector
	vec3 diff; // diffuse constant vector
	vec3 spec; // specular constant vector
};

// point light
struct Point
{
	vec3 pos; // position of point light
	vec3 amb;
	vec3 diff;
	vec3 spec;

	// attenuation term by distance
	float a; 
	float b;
	float c;
};

// spot light
/*
struct Spot
{
	vec3 pos;
	vec3 amb;
	vec3 diff;
	vec3 spec;

	vec3 dir;
	vec3 cutoffstart; // intensity falls down near the boundary
	vec3 cutoffend; // intensity goes to 0 after exceeding cutoffend angle

	// attenuation term by distance
	float a;
	float b;
	float c;
};
*/

uniform Material material;
uniform Directional directional;
uniform Point point;
// uniform Spot spot;

uniform mat4 ModelTransform;
uniform mat4 Eye; // location of the camera
uniform mat4 Projection;

// Ouput data
out vec3 color;

// Assume that the constant for the ambient light is equal to the constant for the diffuse light.
vec3 directionalLight(Directional light, vec3 normal, vec3 viewDirection){
	vec3 lightDirection = normalize(light.dir); // note that the light vector has a negative direction of the light movement.
	vec3 reflection = reflect(lightDirection, normal); // according to phong model
	
	float diff = max(dot(normal, lightDirection), 0.0); // = \bar{cosine}(angle between normal and light)
	float spec = pow(max(dot(viewDirection, reflection), 0.0), material.shininess); // = (reflection \cdot viewDirection)^{shininess}

	vec3 ambient, diffuse, specular;
	ambient = light.amb * fragmentColor;
	diffuse = light.diff * material.diffuseIntensity * diff * fragmentColor;
	specular = light.spec * material.specularIntensity * spec * fragmentColor;
	return (ambient + diffuse + specular);
}

vec3 pointLight(Point light, vec3 normal, vec3 viewDirection){
	vec3 lightDirection = normalize(light.pos - fragmentPosition); // imagine that the eye is located at the fragment and see the point light directly
	vec3 reflection = reflect(-lightDirection, normal);
	float dist = length(light.pos - fragmentPosition);
	float attenuation = 1.0f / (light.a + light.b * dist + light.c * dist * dist); // attenuation factor

	float diff = max(dot(normal, lightDirection), 0.0);
	float spec = pow(max(dot(viewDirection, reflection), 0.0), material.shininess); // = (reflection \cdot viewDirection)^{shininess}

	vec3 ambient, diffuse, specular;
	ambient = light.amb * fragmentColor;
	diffuse = light.diff * material.diffuseIntensity * diff * fragmentColor;
	specular = light.spec * material.specularIntensity * spec * fragmentColor;
	return (ambient + diffuse * attenuation + specular * attenuation);
}

/*
vec3 spotLight(Spot light, vec3 normal, vec3 viewDirection){
	vec3 lightDirection = normalize(light.pos - fragmentPosition);
	vec3 reflection = reflect(-lightDirection, normal);
	float dist = length(light.pos - fragmentPosition);

	vec3 ambient, diffuse, specular;
	return (ambient + diffuse + specular);
}
*/

void main(){
	vec3 normal = normalize(fragmentNormal);
	vec3 viewDirection = normalize((inverse(Eye) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - fragmentPosition); // direction from eye to the fragment

	// color by directional light
	vec3 result = directionalLight(directional, normal, viewDirection);
	// color by point light
	result += pointLight(point, normal, viewDirection);
	// color by spot light
	// result += spotLight(point, normal, viewDirection);

	color = result;

	// float diffuse = max(0.0, dot(normal, tolight));
	// vec3 intensity = fragmentColor * diffuse;

	// color = pow(intensity, vec3(1.0/2.2)); // Apply gamma correction
}

