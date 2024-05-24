#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
class camera
{
public:
	
	camera();
	void cameraUpdateInput(GLFWwindow* window, float deltaTime);

	inline glm::vec3 eulerToLookVec(glm::vec3 euler) { return glm::vec3(std::cosf(glm::radians(euler.z)) * std::cosf(glm::radians(euler.y)), sinf(glm::radians(euler.y)), std::sinf(glm::radians(euler.z)) * std::cosf(glm::radians(euler.y))); }

	float panningSpeed;
	float translationSpeed;
	float fov;
	float near;
	float far;

	//roll, pitch, yaw
	glm::vec3 eulerRotation;
	glm::vec3 position;
	glm::vec3 lookVector;
	glm::vec3 upVector;
private:
	bool clickDB;
};

