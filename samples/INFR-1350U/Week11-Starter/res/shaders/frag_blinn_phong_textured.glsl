#version 410

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

uniform sampler2D s_Diffuse;
uniform sampler2D s_Diffuse2;
uniform sampler2D s_Specular;

uniform vec3  u_AmbientCol;
uniform float u_AmbientStrength;

uniform int u_lightingStrength;
uniform int u_diffuseBool; 

uniform vec3  u_LightPos;
uniform vec3  u_LightCol;
uniform float u_AmbientLightStrength;
uniform float u_SpecularLightStrength;
uniform float u_Shininess;

uniform float u_Time;
uniform float u_EffectBool;
// NEW in week 7, see https://learnopengl.com/Lighting/Light-casters for a good reference on how this all works, or
// https://developer.valvesoftware.com/wiki/Constant-Linear-Quadratic_Falloff
uniform float u_LightAttenuationConstant;
uniform float u_LightAttenuationLinear;
uniform float u_LightAttenuationQuadratic;

uniform float u_TextureMix;

uniform vec3  u_CamPos;

out vec4 frag_color;

// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
void main() {
	// Lecture 5

		//added effect
	float LSD = (sin(u_Time) * u_EffectBool) + 1;
	float LCD = (cos(u_Time) * u_EffectBool) + 1;

	vec3 lightCol = u_LightCol * 0.5 + vec3(LCD - 1, LSD - 1, 0.0) * 0.5;

	vec3 ambient = u_AmbientLightStrength * lightCol;


	// Diffuse
	vec3 N = normalize(inNormal);
	vec3 lightDir = normalize(u_LightPos - inPos);

	float dif = max(dot(N, lightDir), 0.0);
	vec3 diffuse = (dif * lightCol * u_diffuseBool);// add diffuse intensity

	//Attenuation
	float dist = length(u_LightPos - inPos);

	// Specular
	vec3 viewDir  = normalize(u_CamPos - inPos);
	vec3 h        = normalize(lightDir + viewDir);

	// Get the specular power from the specular map
	float texSpec = texture(s_Specular, inUV).x;
	float spec = pow(max(dot(N, h), 0.0), u_Shininess); // Shininess coefficient (can be a uniform)
	vec3 specular = u_SpecularLightStrength * texSpec * spec * lightCol; // Can also use a specular color



	// Get the albedo from the diffuse / albedo map
	vec4 textureColor = texture(s_Diffuse, inUV * LSD);


	vec3 result = (
		(u_AmbientCol * u_AmbientStrength) + // global ambient light
		(ambient + diffuse + specular) /* * attenuation*/ // light factors from our single light
		) * inColor * (textureColor.rgb); // Object color

	frag_color = vec4(result, textureColor.a);
}