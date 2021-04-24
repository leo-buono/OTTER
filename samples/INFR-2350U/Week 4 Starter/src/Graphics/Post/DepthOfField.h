#pragma once

#include "Graphics/Post/PostEffect.h" 
#include <GLM/glm.hpp>

class DepthOfFieldEffect : public PostEffect
{
public:
	//Initializes framebuffer
	//Overrides post effect Init
	void Init(unsigned width, unsigned height) override;

	//Applies the effect to this buffer
	//passes the previous framebuffer with the texture to apply as parameter
	void ApplyEffect(PostEffect* buffer) override;

	void SetThreshold(float x);
	float GetThreshold();

	void SetBlur(float x[5]); 
	float* GetBlur();

	void SetFocalDistance(float x);
	float GetFocalDist();


	void SetFocalLength(float x);
	float GetFocalLength();


	void SetAperture(float x);
	float GetAperture();
	 

	void SetMaxCoC(float x);
	float GetMaxCoC();

	void SetFarZ(float x);
	float GetFarZ();

	void SetNearZ(float x);
	float GetNearZ();

	void SetPlaneInFocus(float x);
	float GetPlaneInFocus();

private:
	
	float _width;
	float _height;


	int _passes = 10;
	float _threshold = 0.1f;
	float _S1 = 20.0f;  
	float _F = 0.0303f;
	float _A = 0.6062f;
	float _MAXCoC = 1.0f;
	float _zFar = 50.f;
	float _zNear = 0.1f;
	float _planeInFocus = 10.f;
	//float _threshold = 0.1f;
	glm::vec4 _lightPos = glm::vec4(0);
	glm::vec4 _colour = glm::vec4(0);
	glm::vec2 _pixelSize;
	float _blurValues[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 }; 

};