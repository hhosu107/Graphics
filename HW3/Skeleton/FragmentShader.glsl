#version 330 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;
// Interpolated values from the vertex shaders
in vec3 fragmentColor;
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

mat4 NormalMatrix(mat4 MVM)
{
	mat4 invm = inverse(MVM);
	invm[0][3] = 0;
	invm[1][3] = 0;
	invm[2][3] = 0;

	return transpose(invm);
}

vec3 directionalLight(Directional light, vec3 normal, vec3 viewDirection){
	// light vector
	vec3 lightDirection = normalize(light.dir); 
	float spec;
	
	// diffuse and specular
	float diff = max(dot(normal, lightDirection), 0.0); 
	if(blinn == 1){
		vec3 halfVec = normalize(lightDirection + viewDirection);
		spec = pow(max(dot(normal, halfVec), 0.0), material.alpha);
	}
	else{
	vec3 reflection = reflect(-lightDirection, normal); 
	spec = pow(max(dot(viewDirection, reflection), 0.0), material.alpha);
	}
	vec3 diffuse, specular;
	diffuse = d * diff * light.color;
	specular = s * spec * light.color;
	return (diffuse + specular);
}

// TODO: change coolColor, warmColor, alpha, beta as uniform variables of fragment shader: preset them in the colormodel.
vec3 pointLight(Point light, vec3 normal, vec3 viewDirection){
	// imagine that the eye is located at the fragment and see the point light directly (world space)
	vec3 lightDirection = normalize(light.pos - fragmentWorldPosition);
	float spec;
	float dist = length(light.pos - fragmentWorldPosition); // distance on the world space
	float attenuation = 1.0f / (light.a + light.b * dist + light.c * dist * dist); // attenuation factor

	// diffuse and specular
	// HW2: phong(blinn-phong) shading
	// float diff = max(dot(normal, lightDirection), 0.0);
	// HW3: Gooch Shading
	float diff = (1.0 + dot(normal, lightDirection))/2;
	
	if (diff >= 0.93){
		diff = 1.0;
	}
	else if(diff >= 0.78){
		diff = 0.85;
	}
	else if(diff >= 0.64){
		diff = 0.7;
	}
	else if(diff >= 0.45){
		diff = 0.55;
	}
	else if(diff >= 0.35){
		diff = 0.4;
	}
	else if(diff >= 0.18){
		diff = 0.25;
	}
	else if(diff >= 0.05){
		diff = 0.1;
	}
	else diff = 0;
	
	/* HW2: phong(blinn-phong) shading
	if(blinn == 1){
	vec3 halfVec = normalize(lightDirection + viewDirection);
	spec = pow(max(dot(normal, halfVec), 0.0), material.alpha);
	}
	else{
	vec3 reflection = reflect(-lightDirection, normal); // according to phong model
	spec = pow(max(dot(viewDirection, reflection), 0.0), material.alpha); // = (reflection \cdot viewDirection)^{alpha}
	}
	
	// light computation with attenuation
	vec3 diffuse, specular;
	diffuse = d * diff * light.color;
	specular = s * spec * light.color;
	return (diffuse * attenuation + specular * attenuation);
	*/
	// HW3: Gooch shading
	// cool color mixed with color of the object
	// vec3 coolColorMod = coolColor + fragmentColor * alpha;
	vec3 coolColorMod = vec3(0.0, 0.7, 0.0) + fragmentColor * 0.3;
    // warm color mixed with color of the object
	// vec3 warmColorMod = warmColor + fragmentColor * beta;
    vec3 warmColorMod = vec3(0.6, 0.0, 0.6) + fragmentColor * 0.4;
    // interpolation of cool and warm colors according 
    // to lighting intensity. The lower the light intensity,
    // the larger part of the cool color is used
    return attenuation * mix(coolColorMod, warmColorMod, diff);
}

vec3 blinnPointLight(Point light, vec3 normal, vec3 viewDirection){
	// imagine that the eye is located at the fragment and see the point light directly (world space)
	vec3 lightDirection = normalize(light.pos - fragmentWorldPosition);
	float spec;
	float dist = length(light.pos - fragmentWorldPosition); // distance on the world space
	float attenuation = 1.0f / (light.a + light.b * dist + light.c * dist * dist); // attenuation factor

	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 halfVec = normalize(lightDirection + viewDirection);
	spec = pow(max(dot(normal, halfVec), 0.0), material.alpha);
	
	// light computation with attenuation
	vec3 diffuse, specular;
	diffuse = d * diff * light.color;
	specular = s * spec * light.color;
	return (diffuse * attenuation + specular * attenuation);
}


vec3 spotLight(Spot light, vec3 normal, vec3 viewDirection){
	// imagine that the eye is located at the fragment and see the point light directly (world space)
	vec3 lightDirection = normalize(light.pos - fragmentWorldPosition);
	float spec;
	float dist = length(light.pos - fragmentWorldPosition); // distance on the world space
	float attenuation = 1.0f / (light.a + light.b * dist + light.c * dist * dist); // attenuation factor

	// diffuse and specular
	float diff = max(dot(normal, lightDirection), 0.0);
	if(blinn == 1){
	vec3 halfVec = normalize(lightDirection + viewDirection);
	spec = pow(max(dot(normal, halfVec), 0.0), material.alpha);
	}
	else{
	vec3 reflection = reflect(-lightDirection, normal); // according to phong model
	spec = pow(max(dot(viewDirection, reflection), 0.0), material.alpha); // = (reflection \cdot viewDirection)^{alpha}
	}
	// light computation with attenuation
	vec3 diffuse, specular;
	diffuse = d * diff * light.color;
	specular = s * spec * light.color;
	return (diffuse * attenuation + specular * attenuation);
}

void main(){
	// fragment normal on world space
	vec3 normal = normalize(fragmentWorldNormal);
	// fragment view direction on world space: first represent the eye on the world coordinate and then compute the view vector
	vec3 viewDirection = -normalize((inverse(Eye) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - fragmentWorldPosition); 
	// color by directional light
	vec3 intensity = vec3(0.01, 0.01, 0.01); // ambient term initialization
	/* HW2: phong(blinn-phong) shading
	for(int i=0; i<3; i++){
		intensity = intensity + (light[i]) * directionalLight(directional[i], normal, viewDirection);
	}
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
	*/
	if(blinn == 0){
		intensity = pointLight(point[1], normal, viewDirection);
		color = intensity;
	}
	else{
		intensity = intensity + blinnPointLight(point[1], normal, viewDirection);
		color = pow(fragmentColor * intensity, vec3(1.0/2.2));
	}
}


