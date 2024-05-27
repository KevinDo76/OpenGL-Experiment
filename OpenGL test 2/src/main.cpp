#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "basicOpenGL/vertexBuffer.h"
#include "basicOpenGL/indexBuffer.h"
#include "basicOpenGL/vertexArray.h"
#include "basicOpenGL/vaoElementLayout.h"
#include "basicOpenGL/shader.h"
#include "basicOpenGL/texture.h"
#include "basicOpenGL/mesh.h"
#include "basicOpenGL/objParser.h"
#include "basicOpenGL/camera.h"
#include "basicOpenGL/material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << "[OpenGL Error](" << type << ")(" << id << ")(" << severity << ") " << message << std::endl;
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout<<"glfw init fail\n";
        return -1;
    }

    int width = 1920;
    int height = 1080;


    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        std::cout << "glfw windows fail\n";
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLenum respond = glewInit();
    if (respond != GLEW_OK) {
        std::cout << "Glew: "<< glewGetErrorString(respond);
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB); 
    //glEnable(GL_CULL_FACE);

   

    texture textureObj("resources/textures/backroom3.png", "resources/textures/backroom3normal.png");

    shader shaderProgram("resources/shaders/default.vert", "resources/shaders/default.frag", 2, 2, "resources/shaders/default.geom");
    shaderProgram.setUniform1i("u_Texture", 0);
    shaderProgram.setUniform1i("u_Normal", 1);


    shaderProgram.lightDirArray[0].position = glm::vec3(0, 20, 0);
    shaderProgram.lightDirArray[0].color = glm::vec3(0.839215686f, 0.870588235f, 0.5f);
    shaderProgram.lightDirArray[0].activated = true;
    shaderProgram.lightDirArray[0].lightPower = 200;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    shaderProgram.unbind();
    textureObj.unbind();

    mesh Teapot("resources/object/backroom3.obj");
    //mesh abby("resources/object/abby.txt");
    camera cameraObj;
    material mat(glm::vec3(1), glm::vec3(1), glm::vec3(1), 32);


    /* Loop until the user closes the window */
    float yaw = 0;
    float pitch = 0;

    double lastTime = glfwGetTime();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    float lightPow = 10;

    glm::vec3 rotationCube(0);

    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();
        double currenTime = glfwGetTime();
        double deltaTime = currenTime - lastTime;
        lastTime = currenTime;

        //shaderProgram.lightDirArray[0].position = cameraObj.position;
        shaderProgram.lightDirArray[0].position = cameraObj.position+glm::vec3(0,2,0);
        shaderProgram.lightDirArray[0].direction = cameraObj.lookVector;
        
        if (!io.WantCaptureMouse)
        {
            cameraObj.cameraUpdateInput(window, deltaTime);
        }

        Teapot.rotation = rotationCube;
        

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);
        
        //Teapot.rotation = glm::vec3(count/2.f, count/2.f, 0);
        Teapot.scale = glm::vec3(3);
        //abby.scale = glm::vec3(20);

        textureObj.bind();
        Teapot.draw(shaderProgram, cameraObj, glm::vec2(width, height), mat);
        //abby.draw(shaderProgram, cameraObj, glm::vec2(width, height));
        shaderProgram.setUniform3f("material.ambient", mat.ambient.x, mat.ambient.y, mat.ambient.z);

        shaderProgram.lightDirArray[0].lightPower = lightPow;

        ImGui::Begin("ImGui test");
        ImGui::Text("Open GL test");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0 / double(ImGui::GetIO().Framerate), double(ImGui::GetIO().Framerate));
        ImGui::Text("Camera Position: (%.3f, %.3f, %.3f)", (double)cameraObj.position.x, (double)cameraObj.position.y, (double)cameraObj.position.z);
        ImGui::Text("Camera Look Vector: (%.3f, %.3f, %.3f)", (double)cameraObj.lookVector.x, (double)cameraObj.lookVector.y, (double)cameraObj.lookVector.z);
        ImGui::SliderFloat("Light Power", &lightPow, 0, 1000);
        ImGui::Text("Cube Rotation:");
        ImGui::SliderFloat("X", &rotationCube.x, -180, 180);
        ImGui::SliderFloat("Y", &rotationCube.y, -180, 180);
        ImGui::SliderFloat("Z", &rotationCube.z, -180, 180);
        ImGui::End();

        //cameraObj.position.y = 1.8*2;

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //std::cout << deltaTime *1000 << " " << cameraObj.position.y << "\n";
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}