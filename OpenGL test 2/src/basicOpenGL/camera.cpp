#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
camera::camera()
    :fov(45), near(1.f), far(1000.0f), lookVector(1, 0, 0), position(0), upVector(0, 1, 0), translationSpeed(10), panningSpeed(10000), eulerRotation(0), clickDB(false)
{
}

void camera::cameraUpdateInput(GLFWwindow* window, float deltaTime)
{
    double xpos, ypos;
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
    {
        if (!clickDB)
        {
            clickDB = true;
            glfwSetCursorPos(window, width / 2, height / 2);
        }
        glfwGetCursorPos(window, &xpos, &ypos);
        glfwSetCursorPos(window, width / 2, height / 2);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
        {
            float rotX = (float)((float)xpos - (width / 2)) / width;
            float rotY = (float)((float)ypos - (height / 2)) / height;

            eulerRotation.z += rotX * panningSpeed * deltaTime;
            eulerRotation.y += rotY * -panningSpeed * deltaTime;
            eulerRotation.y = std::fminf(std::fmax(eulerRotation.y, -89.99), 89.99);
            lookVector = eulerToLookVec(eulerRotation);

        }
    }
    else 
    {
        clickDB = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
   
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += lookVector * translationSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= lookVector * translationSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= glm::normalize(glm::cross(lookVector, upVector)) * translationSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += glm::normalize(glm::cross(lookVector, upVector)) * translationSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position += upVector * translationSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        position -= upVector * translationSpeed * deltaTime;
    }

}
