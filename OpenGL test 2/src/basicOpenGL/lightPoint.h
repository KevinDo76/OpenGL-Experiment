#pragma once
#include <glm/glm.hpp>
class lightPoint
{
public:
	bool activated;
	float lightPower;
	glm::vec3 position;
	glm::vec3 color;

	
	//glm::vec3 ambient;
	//glm::vec3 diffuse;
	//glm::vec3 specular;

	lightPoint();
private:

};

