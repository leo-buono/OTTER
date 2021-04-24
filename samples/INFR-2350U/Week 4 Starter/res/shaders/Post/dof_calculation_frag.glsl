#version 420

uniform vec4 u_lightPos;
uniform vec4 u_colour;


// layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
// layout(location = 2) in vec3 inNormal;
layout(location = 0) in vec2 inUV;

layout(binding = 0) uniform sampler2D depthBuffer;
layout(binding = 1) uniform sampler2D screen;

uniform float u_depthEye;

//Focal distance
uniform float S1;
//focal length
uniform float F;
//aperture
uniform float A;
//Max Circle of confusion distance
uniform float MAXCoC;

uniform float zFar;
uniform float zNear;

uniform float u_WindowWidth;
uniform float u_WindowHeight;

uniform float u_planeInFocus;

out vec4 frag_color;

float linearize_depth(float d, float _zNear, float _zFar)
{
    float z_n = 2.0 * d - 1.0;
    return 2.0 * _zNear * _zFar / (_zFar + _zNear - z_n * (_zFar - _zNear));
}

float calculateCircleOfConfusion()
{
    // S2 is the distance this pixel is from the camera
    // This value is computed for you in the default_v vertex shader No its not here is the math
    vec2 screenUV = vec2(gl_FragCoord.x / u_WindowWidth, gl_FragCoord.y / u_WindowHeight);
    vec4 depthTexture = texture(depthBuffer, screenUV);

    //float objectDistance = linearize_depth(depthTexture.r, zNear, zFar);

    float objectDistance = u_depthEye;

    //float CoCScale = (A * F * u_planeInFocus * ( zFar - zNear)) / ((u_planeInFocus - F) * zNear *  zFar);

    //float CoCBias = (A * F * (zNear - u_planeInFocus)) / ((u_planeInFocus *  F) * zNear);

    //float CoC = abs(depthTexture.r * CoCScale + CoCBias);
    
    float CoC = abs(A * (F * (objectDistance - u_planeInFocus)) / (objectDistance * (u_planeInFocus - F)));

    return CoC;
}
void main()
{
    vec4 scene = texture(screen, inUV);
    //frag_color.rgb = scene.rgb;
    // The channela contains the pixel's blurriness scaled up to 255 to how obvious it is
    frag_color.r = calculateCircleOfConfusion();
}