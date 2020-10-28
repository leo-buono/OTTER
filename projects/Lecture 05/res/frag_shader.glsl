#version 410

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;

uniform vec3 LightPos;



out vec4 frag_color;


void main() 
{
	vec3 lightColour = vec3(1.0, 1.0, 1.0);
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColour * inColor;
	//Diffuse
	vec3 N = normalize(inNormal);
	vec3 lightDir = normalize(LightPos - inPos);

	float dif = max(dot(N, lightDir), 0.0);
	vec3 diffuse = dif * inColor;

	float dist = length(LightPos - inPos);
	diffuse = diffuse / dist; //(dist * dist)

	//specular highlights
	vec3 camPos = vec3(0.0, 0.0, 3.0); //pass this as a uniform from your c++ code
	float specularStrength = 6.0;
    vec3 camDir = normalize(camPos - inPos);
//	vec3 reflectDir = reflect(-lightDir, N);
//	float spec = pow(max(dot(camDir, reflectDir), 0.0), 128); //shininess coefficient
//blinn phong lighting
	vec3 halfway = normalize(camDir + lightDir);
	float spec = pow(max(dot(halfway, N), 0.0), 128);

	vec3 specular = specularStrength * spec * lightColour; //can also use a specular colour

	vec3 result = (ambient + diffuse + specular);


	frag_color = vec4(result, 1.0);
}