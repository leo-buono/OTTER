#version 420

layout(location = 0) in vec2 inUV;

out vec4 frag_color;


// layout(binding = 0) uniform sampler2D scene;
layout(binding = 0) uniform sampler2D depthMap;

void main()
{             
    vec4 depth = texture(depthMap, inUV);
    frag_color = vec4(depth.rgb, 1.0);
}