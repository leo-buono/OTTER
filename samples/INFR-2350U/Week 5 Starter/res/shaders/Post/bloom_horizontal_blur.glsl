#version 420

layout(location = 0) in vec2 inUV;

out vec4 frag_color;

layout (binding = 0) uniform sampler2D s_screenTex;

const float weights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

uniform float u_TexelSize;
uniform float u_Spread;

//done with the guidance of https://learnopengl.com/Advanced-Lighting/Bloom and spritelib bloom shaders
//also https://pingpoli.medium.com/the-bloom-post-processing-effect-9352fa800caf for the spread adjustability
//This was taken from the Mecha-Mayhem engine, to complete this final
void main() 
{
	vec3 result = texture(s_screenTex, inUV).rgb * weights[0];
	
	result += texture(s_screenTex, vec2(inUV.x - u_TexelSize * u_Spread * 4, inUV.y)).rgb * weights[4];
	result += texture(s_screenTex, vec2(inUV.x - u_TexelSize * u_Spread * 3, inUV.y)).rgb * weights[3];
	result += texture(s_screenTex, vec2(inUV.x - u_TexelSize * u_Spread * 2, inUV.y)).rgb * weights[2];
	result += texture(s_screenTex, vec2(inUV.x - u_TexelSize * u_Spread    , inUV.y)).rgb * weights[1];
	result += texture(s_screenTex, vec2(inUV.x + u_TexelSize * u_Spread    , inUV.y)).rgb * weights[1];
	result += texture(s_screenTex, vec2(inUV.x + u_TexelSize * u_Spread * 2, inUV.y)).rgb * weights[2];
	result += texture(s_screenTex, vec2(inUV.x + u_TexelSize * u_Spread * 3, inUV.y)).rgb * weights[3];
	result += texture(s_screenTex, vec2(inUV.x + u_TexelSize * u_Spread * 4, inUV.y)).rgb * weights[4];

	frag_color = vec4(result, 1.0);
}