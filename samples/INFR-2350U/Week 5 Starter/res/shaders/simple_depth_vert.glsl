#version 410

layout (location = 0) in vec3 inPosition;

//Out lightspace matrix
uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Model;


void main()
{ 
	//lightspace matrix is the viewprojection matric from the lights perspecive
	gl_Position = u_LightSpaceMatrix * u_Model * vec4(inPosition, 1.0);
}