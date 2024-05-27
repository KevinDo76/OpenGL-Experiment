#include "mesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "vertexArray.h"
#include "vaoElementLayout.h"
#include <iostream>
#include "indexBuffer.h"
#include "GL/glew.h"
#include "objParser.h"
#include "shader.h"
#include <array>
#include "material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static void readFile(const std::string& path, std::string& text)
{
    std::ifstream fileHandle(path.c_str());
    std::string line;
    if (!fileHandle.is_open()) {
        std::cout << "Failed to read file \"" << path << "\"\n";
        return;
    }
    while (getline(fileHandle, line)) {
        text += line + "\n";
    }
}

mesh::mesh(const std::string& objFilePath)
    :indicesCount(0), position(0), rotation(0), scale(1)
{
    std::vector<float> vertexDataBuff;
    std::vector<unsigned int> vertexIndicesBuff;
    
    objParser::parse(objFilePath, vertexDataBuff, vertexIndicesBuff);

    vb = new vertexBuffer(&vertexDataBuff[0], vertexDataBuff.size() * sizeof(float));

    vaoElementLayout vaoLayout;
    vaoLayout.pushElement<float>(3, false);
    vaoLayout.pushElement<float>(2, false);
    vaoLayout.pushElement<float>(3, false);
    vao = new vertexArray(vaoLayout, *vb);

    ib = new indexBuffer(&vertexIndicesBuff[0], vertexIndicesBuff.size() * sizeof(unsigned int));

    indicesCount = vertexIndicesBuff.size();

    vertexIndicesBuff.clear();
    vertexDataBuff.clear();
    std::cout << "texture uploaded\n";
}

mesh::~mesh()
{
    delete vb;
    delete ib;
    delete vao;
}

void mesh::draw(shader& shaderProgram, camera& cameraObj, glm::vec2 dimension, material& mat)
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 modelInverse;
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);
    glm::mat4 MV;

    //model transformation
    model = glm::scale(model, scale);
    //Euler xyz?
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(-rotation.y), glm::vec3(0, 0, 1));
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    //model = glm::rotate(model, glm::radians(1.f), rotation);
    model = glm::translate(model, position);

    //camera transformation
    view = glm::lookAt(cameraObj.position, cameraObj.position + cameraObj.lookVector, cameraObj.upVector);
    
    //view = glm::translate(view, cameraObj.position);
    
    //projection matrix creation
    proj = glm::perspective(glm::radians(cameraObj.fov), (float)(dimension.x/std::fmaxf(1,dimension.y)), cameraObj.near, cameraObj.far);
    
    modelInverse = glm::transpose(glm::inverse(model));
    //final matrix

    MV = view * model;
    shaderProgram.bind();
    shaderProgram.setUniformMat4fv("u_Model", glm::value_ptr(model));
    shaderProgram.setUniformMat4fv("u_Projection", glm::value_ptr(proj));
    shaderProgram.setUniformMat4fv("u_MV", glm::value_ptr(MV));
    shaderProgram.setUniform3f("viewPos", cameraObj.position.x, cameraObj.position.y, cameraObj.position.z);
    shaderProgram.setUniformMat4fv("u_InverseModel", glm::value_ptr(modelInverse));

    shaderProgram.setUniform3f("material.ambient", mat.ambient.x, mat.ambient.y, mat.ambient.z);
    shaderProgram.setUniform3f("material.diffuse", mat.diffuse.x, mat.diffuse.y, mat.diffuse.z);
    shaderProgram.setUniform3f("material.specular", mat.specular.x, mat.specular.y, mat.specular.z);
    shaderProgram.setUniform1f("material.shininess", mat.shininess);

    for (int i = 0; i < shaderProgram.lightPointArray.size(); i++)
    {
        glm::vec3 ambient = (shaderProgram.lightPointArray[i].color * shaderProgram.lightPointArray[i].lightPower);
        glm::vec3 diffuse = shaderProgram.lightPointArray[i].color * shaderProgram.lightPointArray[i].lightPower;
        glm::vec3 specular = shaderProgram.lightPointArray[i].color * shaderProgram.lightPointArray[i].lightPower;

        ambient *= 0.05;
        specular *= 0.5;

        shaderProgram.setUniform1i("p_light[" + std::to_string(i) + "].activated", ((int)shaderProgram.lightPointArray[i].activated));
        shaderProgram.setUniform3f("p_light[" + std::to_string(i) + "].ambient", ambient.r, ambient.g, ambient.b);
        shaderProgram.setUniform3f("p_light[" + std::to_string(i) + "].diffuse", diffuse.r, diffuse.g, diffuse.b);
        shaderProgram.setUniform3f("p_light[" + std::to_string(i) + "].specular", specular.r, specular.g, specular.b);
        shaderProgram.setUniform3f("p_light[" + std::to_string(i) + "].position", shaderProgram.lightPointArray[i].position.r, shaderProgram.lightPointArray[i].position.g, shaderProgram.lightPointArray[i].position.b);
    }

    for (int i = 0; i < shaderProgram.lightDirArray.size(); i++)
    {
        glm::vec3 ambient = (shaderProgram.lightDirArray[i].color * shaderProgram.lightDirArray[i].lightPower);
        glm::vec3 diffuse = shaderProgram.lightDirArray[i].color * shaderProgram.lightDirArray[i].lightPower;
        glm::vec3 specular = shaderProgram.lightDirArray[i].color * shaderProgram.lightDirArray[i].lightPower;

        ambient *= 0.05;
        specular *= 0.5;


        shaderProgram.setUniform1i("d_light[" + std::to_string(i) + "].activated", ((int)shaderProgram.lightDirArray[i].activated));
        shaderProgram.setUniform3f("d_light[" + std::to_string(i) + "].ambient", ambient.r, ambient.g, ambient.b);
        shaderProgram.setUniform3f("d_light[" + std::to_string(i) + "].diffuse", diffuse.r, diffuse.g, diffuse.b);
        shaderProgram.setUniform3f("d_light[" + std::to_string(i) + "].specular", specular.r, specular.g, specular.b);
        shaderProgram.setUniform3f("d_light[" + std::to_string(i) + "].position", shaderProgram.lightDirArray[i].position.r, shaderProgram.lightDirArray[i].position.g, shaderProgram.lightDirArray[i].position.b);
        shaderProgram.setUniform1f("d_light[" + std::to_string(i) + "].cutOffIn", glm::cos(glm::radians((shaderProgram.lightDirArray[i].cutOffIn))));
        shaderProgram.setUniform1f("d_light[" + std::to_string(i) + "].cutOffOut", glm::cos(glm::radians((shaderProgram.lightDirArray[i].cutOffOut))));
        shaderProgram.setUniform3f("d_light[" + std::to_string(i) + "].direction", shaderProgram.lightDirArray[i].direction.x, shaderProgram.lightDirArray[i].direction.y, shaderProgram.lightDirArray[i].direction.z);
    }

    //shaderProgram.setUniform1f("p_light[0].activated", 1.f);   
    //shaderProgram.setUniform3f("p_light[0].ambient", 2.0f, 2.0f, 2.0f);
    //shaderProgram.setUniform3f("p_light[0].diffuse", 500, 500, 500); // darken diffuse light a bit
    //shaderProgram.setUniform3f("p_light[0].specular", 50.f, 50.f, 50.f);
    //shaderProgram.setUniform3f("p_light[0].position", 0, 70, -5);
    //
    //shaderProgram.setUniform1f("p_light[0].constant", 1.0f);
    //shaderProgram.setUniform1f("p_light[0].linear", 0.002f);
    //shaderProgram.setUniform1f("p_light[0].quadratic", 0.001f);
    //
    //
    //shaderProgram.setUniform1f("p_light[1].activated", 1.f);
    //shaderProgram.setUniform3f("p_light[1].ambient", 0.2f, 0.2f, 0.2f);
    //shaderProgram.setUniform3f("p_light[1].diffuse", 500, 0, 0); // darken diffuse light a bit
    //shaderProgram.setUniform3f("p_light[1].specular", 50.0f, 0.0f, 0.0f);
    //shaderProgram.setUniform3f("p_light[1].position", 0, -7, -5);
    //
    //shaderProgram.setUniform1f("p_light[1].constant", 1.0f);
    //shaderProgram.setUniform1f("p_light[1].linear", 0.002f);
    //shaderProgram.setUniform1f("p_light[1].quadratic", 0.001f);

    (*vao).bind();
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
    shaderProgram.unbind();
}
