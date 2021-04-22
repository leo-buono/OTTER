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

private:
	
	int _passes = 100;
	float _threshold = 0.1f;
	//float _threshold = 0.1f;
	glm::vec4 _lightPos = glm::vec4(0);
	glm::vec4 _colour = glm::vec4(0);
	glm::vec2 _pixelSize;
	float _blurValues[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 }; 

};