#version 420
out vec4 frag_color;
  
layout(location = 0) in vec2 inUV;


layout(binding = 0) uniform sampler2D scene;
layout(binding = 1) uniform sampler2D bloomBlur;
uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    vec4 hdrColor = texture(scene, inUV);      
    vec4 bloomColor = texture(bloomBlur, inUV);
    
    frag_color = 1.0 - (1.0 - hdrColor) * (1.0 - bloomColor);
}  