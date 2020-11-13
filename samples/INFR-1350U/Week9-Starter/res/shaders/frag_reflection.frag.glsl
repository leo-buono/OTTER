#version 410

uniform samplerCube s_Environment;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;


uniform sampler2D s_Diffuse;
uniform sampler2D s_Diffuse2;
uniform sampler2D s_Specular;
uniform sampler2D s_textureRound2;

uniform vec3  u_AmbientCol;
uniform float u_AmbientStrength;

uniform vec3  u_LightPos;
uniform vec3  u_LightCol;
uniform float u_AmbientLightStrength;
uniform float u_SpecularLightStrength;
uniform float u_Shininess;
// NEW in week 7, see https://learnopengl.com/Lighting/Light-casters for a good reference on how this all works, or
// https://developer.valvesoftware.com/wiki/Constant-Linear-Quadratic_Falloff
uniform float u_LightAttenuationConstant;
uniform float u_LightAttenuationLinear;
uniform float u_LightAttenuationQuadratic;

uniform float u_TextureMix;



uniform mat3 u_EnvironmentRotation;

uniform vec3  u_CamPos;

out vec4 frag_color;

// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
void main() {

	vec3 ambient = u_AmbientLightStrength * u_LightCol;

	vec3 N = normalize(inNormal);
	vec3 lightDir = normalize(u_LightPos - inPos);

	float dif = max(dot(N, lightDir), 0.0);
	vec3 diffuse = dif * u_LightCol;// add diffuse intensity

	//Attenuation
	float dist = length(u_LightPos - inPos);
	float attenuation = 1.0f / (
		u_LightAttenuationConstant + 
		u_LightAttenuationLinear * dist +
		u_LightAttenuationQuadratic * dist * dist);

	// Specular
	vec3 viewDir  = normalize(u_CamPos - inPos);
	vec3 h        = normalize(lightDir + viewDir);

	// Get the specular power from the specular map
	float texSpec = texture(s_Specular, inUV).x;
	float spec = pow(max(dot(N, h), 0.0), u_Shininess); // Shininess coefficient (can be a uniform)
	vec3 specular = u_SpecularLightStrength * texSpec * spec * u_LightCol; // Can also use a specular color

	// Get the albedo from the diffuse / albedo map
	vec4 textureColor1 = texture(s_Diffuse, inUV);
	vec4 textureLol = texture(s_textureRound2, inUV);
	vec4 textureColor = mix(textureColor1, textureLol, u_TextureMix);
	//We don't talk about how unoptimised it is, just like we don't talk 
	//about the 300 lines in main.cpp


	vec3 toEye = normalize(inPos - u_CamPos);
	vec3 reflected = reflect(toEye, N);
	// Look up the environment texture
	vec3 environment = texture(s_Environment, u_EnvironmentRotation * reflected).rgb;
	// For now just return the result, fully reflective!
	
	vec3 result = mix(textureColor.rgb, environment, textureLol.g);

		frag_color = vec4(result, textureLol.g);

	
}