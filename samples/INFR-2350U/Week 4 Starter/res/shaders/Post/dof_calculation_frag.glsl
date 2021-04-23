#version 420

uniform vec4 u_lightPos;
uniform vec4 u_colour;


// layout(location = 0) in vec3 inPos;
// layout(location = 1) in vec3 inColor;
// layout(location = 2) in vec3 inNormal;
layout(location = 0) in vec2 inUV;

layout(binding = 1) uniform sampler2D depthBuffer;

uniform float u_depthEye;

//Focal distance
uniform float S1;
//focal length
uniform float F;
//aperture
uniform float A;
//Max Circle of confusion distance
uniform float MAXCoC;

out vec4 frag_color;

float calculateCircleOfConfusion()
{
    // S2 is the distance this pixel is from the camera
    // This value is computed for you in the default_v vertex shader No its not here is the math
    vec4 depthTexture = texture(depthBuffer, inUV);



    //Yes this is the straight from the discord
    float S2 = abs(depthTexture.g);

    float CoC = A  * (abs(S2 - S1) / S2) * (F / (S1 - F));

    float SensorHeight = 0.024f; // 24 mm

    float PercentOfSensor =    CoC / SensorHeight;

    float BlurFactor = clamp(PercentOfSensor, 0.0f, MAXCoC);

    return BlurFactor;
}

// Calculate the color of the pixel, you do not need to modify this function
// vec3 calculatePixelColor()
// {
//     vec3 L = normalize(u_lightPos.xyz - inPos);
//     vec3 N = normalize(inNormal);

//     float diffuse = max(0.0, dot(N, L));

//     if (diffuse <= 0.00) diffuse = 0.00;
//     else if (diffuse <= 0.25) diffuse = 0.25;
//     else if (diffuse <= 0.50) diffuse = 0.50;
//     else if (diffuse <= 0.75) diffuse = 0.75;
//     else diffuse = 1.00;

//     return vec3(0.5, 0.5, 0.5) * (diffuse * 0.8f) + u_colour.rgb;
// }

void main()
{
    // // The RGB channels contain the pixel's color
    //frag_color.rgb = calculatePixelColor();

    // The A channel contains the pixel's blurriness
    frag_color.a = calculateCircleOfConfusion();
    //frag_color = frag_color;
}