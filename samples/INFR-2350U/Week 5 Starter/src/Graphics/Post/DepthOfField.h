#pragma once
#include "Graphics/Post/PostEffect.h"

class DepthOfField : public PostEffect
{
public:
	//Initializes framebuffer
//Overrides post effect Init
	void Init(unsigned width, unsigned height) override;

	//Applies the effect to this buffer
	//passes the previous framebuffer with the texture to apply as parameter
	void ApplyEffect(PostEffect* buffer) override;


private:
	glm::vec4 _LightPos = glm::vec4(0, 0, 0, 0);
};
