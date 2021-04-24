#version 420

layout(location = 0) in vec2 inUV;

out vec4 frag_color;


// layout(binding = 0) uniform sampler2D scene;
layout(binding = 0) uniform sampler2D depthMap;

float linearize_depth(float d, float _zNear, float _zFar)
{
  float z_n = 2.0 * d - 1.0;
  return 2.0 * _zNear * _zFar / (_zFar + _zNear - z_n * (_zFar - _zNear));
}


void main()
{             
    vec4 depth = texture(depthMap, inUV);
    frag_color = vec4(depth.rgb, 1.0);
}