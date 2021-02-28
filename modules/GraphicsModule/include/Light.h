#pragma once
#include <GLM/common.hpp>

struct Light
{
	//Light controls
	glm::vec3 _lightCol = glm::vec3(1.0f, 1.0f, 1.0f);

	//Ambience controls
	float _ambientPow = 0.1f;
	glm::vec3 _ambientCol = glm::vec3(1.0f, 1.0f, 1.0f);

	//Power controls
	float _lightAmbientPow = 0.05f;
	float _lightSpecularPow = 1.0f;
};