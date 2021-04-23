#version 420

layout(location = 0) in vec2 inUV;

out vec4 frag_color;

uniform sampler2D s_image;

/*layout(location = 40) in */float outBlurFactor = 1;

layout(binding = 0) uniform sampler2D scene;
layout(binding = 1) uniform sampler2D blurMap;


// uniform float weight1;
// uniform float weight2;
// uniform float weight3;
// uniform float weight4;
// uniform float weight5;

float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{             
    vec4 theBlurMap = texture(blurMap, inUV);

    vec2 tex_offset = 1.0 / textureSize(s_image, 0); // gets size of single texel
    vec3 result = texture(scene, inUV).rgb * (weight[0]); // current fragment's contribution

    //vec3 result = vec3(0,0,0);

    for(int i = 1; i < 5; ++i)
    {
        result += texture(scene, inUV + vec2(tex_offset.x * i * theBlurMap.a, 0.0)).rgb *(weight[i]);
        result += texture(scene, inUV - vec2(tex_offset.x * i * theBlurMap.a, 0.0)).rgb * (weight[i]);
    }
    frag_color = vec4(result, 1.0);
}