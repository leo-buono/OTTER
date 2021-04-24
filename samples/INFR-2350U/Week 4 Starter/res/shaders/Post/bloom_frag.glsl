#version 420

layout(location = 0) in vec2 inUV;

uniform float u_threshold;
uniform float u_strength;
uniform sampler2D s_colorTex;

out vec4 frag_color;


void main()
{            
    vec4 color = texture2D(s_colorTex, inUV);


    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > u_threshold)
        frag_color = vec4(u_strength * color.rgb, 1.0);
    else
    {
      frag_color = vec4(0.0, 0.0, 0.0, 1.0);
    }
}