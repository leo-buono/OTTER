#pragma once
#include <Logging.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <json.hpp>
#include <fstream>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArrayObject.h"
#include "Shader.h"
#include "Camera.h"

class ObjLoader
{
public:
	//contructor
	ObjLoader(const std::string& fileName, bool usingMaterial = false);
	~ObjLoader();

	void LoadMesh(const std::string& fileName, bool usingMaterial = false);

	void Draw(Camera::sptr camera, glm::mat4 transform, glm::mat3 rotation, glm::vec3 colour,
		glm::vec3 lightPos, glm::vec3 lightColour = glm::vec3(1.f), float specularStrength = 128, float shininess = 4.f,
		float ambientLightStrength = 0.05f, glm::vec3 ambientColour = glm::vec3(0.f), float ambientStrength = 0.1f
	);

	Shader::sptr GetShader() { return m_shader; }

private:
	std::vector<VertexArrayObject::sptr> m_vao = {};
	Shader::sptr m_shader = Shader::Create();
	std::vector<size_t> m_numberOfVertices = {};
	bool m_usingMaterial = false;
	glm::vec3 m_ambientColour = glm::vec3(0.f);
	glm::vec3 m_specularColour = glm::vec3(0.f);
};