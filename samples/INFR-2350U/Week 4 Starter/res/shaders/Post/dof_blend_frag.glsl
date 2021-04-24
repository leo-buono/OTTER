#version 420
out vec4 frag_color;
  
layout(location = 0) in vec2 inUV;


layout(binding = 0) uniform sampler2D sceneSharp;
layout(binding = 1) uniform sampler2D sceneBlur;
layout(binding = 2) uniform sampler2D blurMap;
uniform float simulateLerp;

void main()
{             
    vec4 sharpColor = texture(sceneSharp, inUV);      
    vec4 blurColor = texture(sceneBlur, inUV);
    vec4 mapBlur = texture(blurMap, inUV);
    
    //NEED TO DIVIDE BY 255 TO GET A 0 - 1 THINGY MEBOB

    vec4 result = vec4(mix(sharpColor, mapBlur, simulateLerp));
    frag_color = vec4(result.rgb, 1.0);
    //frag_color = bloomColor;
}  