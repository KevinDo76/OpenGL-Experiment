#pragma once
#include <glm/glm.hpp>
class material
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	material(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, float _shininess);
	material();
};