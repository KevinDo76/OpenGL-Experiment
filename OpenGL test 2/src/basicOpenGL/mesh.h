#pragma once
#include <string>
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include <glm/glm.hpp>
#include "shader.h"
#include "camera.h"
#include "material.h"
class mesh
{
public:
	mesh(const std::string& objFilePath);
	~mesh();

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void draw(shader& shaderProgram, camera& cameraObj, glm::vec2 dimension, material& mat);
private:
	vertexBuffer* vb;
	vertexArray* vao;
	indexBuffer* ib;
	int indicesCount;
};

