#include "shader.h"
#include <string>
#include <fstream>
#include <iostream>
#include "GL/glew.h"
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

static void replaceText(std::string& src, const std::string& toReplace, const std::string& replaceWith)
{
    std::size_t pos = src.find(toReplace);
    if (pos == std::string::npos)
    {
        std::cout << "Failed to replace\n";
        throw;
    }
    src.replace(pos, toReplace.length(), replaceWith);
}

static void processFragShader(const std::string& path, std::string& shaderSrc, int pointLightC)
{
    std::string lightPointPreprocess = "|LIGHT_COUNT|";
    std::string lightDirPreprocess = "|DIR_LIGHT_COUNT|";
    std::string lightPointCount = std::to_string(pointLightC);
    std::string lightDirCount = std::to_string(2);


    readFile(path, shaderSrc);

    replaceText(shaderSrc, lightPointPreprocess, lightPointCount);
    replaceText(shaderSrc, lightDirPreprocess, lightDirCount);
}

static unsigned int compileShader(const std::string& src, GLenum type)
{
    
    unsigned int shaderID = glCreateShader(type);
    const char* shaderChar = src.c_str();
    glShaderSource(shaderID, 1, &shaderChar, 0);
    glCompileShader(shaderID);

    int compiled = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE)
    {
        int textLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &textLength);

        GLchar* debugMessage = new GLchar[textLength];
        glGetShaderInfoLog(shaderID, textLength, &textLength, debugMessage);

        glDeleteShader(shaderID);

        std::cout << "Compile error(" << type << "): " << debugMessage << "\n";
        return 0;
    }
    return shaderID;
}

shader::shader(std::string vertexPath, std::string fragPath, float lightPointC, std::string geoPath)
    :lightPointCount(lightPointC)
{

    lightPointArray.reserve(lightPointC);
    for (int i = 0; i < lightPointC; i++)
    {
        lightPointArray.emplace_back();
    }


    std::string vertexSrc;
    std::string fragmentSrc;
    std::string geometrySrc;
    readFile(vertexPath, vertexSrc);
    processFragShader(fragPath, fragmentSrc, lightPointCount);
    if (geoPath.length() > 0)
    {
        readFile(geoPath, geometrySrc);

    }
    else
    {
        std::cout << "No geometry shader\n";
    }
    unsigned int vertexID = compileShader(vertexSrc, GL_VERTEX_SHADER);
    unsigned int fragmentID = compileShader(fragmentSrc, GL_FRAGMENT_SHADER);
    unsigned int geometryID = 0xffffffff;

    unsigned int programID = glCreateProgram();
    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);

    if (geoPath.length() > 0)
    {
        geometryID = compileShader(geometrySrc, GL_GEOMETRY_SHADER);
        glAttachShader(programID, geometryID);
    }

    glLinkProgram(programID);

    int linked;
    glGetProgramiv(programID, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE)
    {
        int textLength = 0;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &textLength);

        GLchar* debugMessage = new GLchar[textLength];
        glGetProgramInfoLog(programID, textLength, &textLength, debugMessage);

        glDeleteProgram(programID);

        std::cout << "Linking error: " << debugMessage << "\n";
    }
    else 
    {
        glDetachShader(programID, vertexID);
        glDetachShader(programID, fragmentID);
        if (geoPath.length() > 0)
        {
            glDetachShader(programID, geometryID);
        }

    }
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
    if (geoPath.length() > 0)
    {
        glDeleteShader(geometryID);
    }
    id = programID;
}

shader::~shader()
{
    glDeleteProgram(id);
}

void shader::bind() const
{
    glUseProgram(id);
}

void shader::unbind() const
{
    glUseProgram(0);
}

int shader::getUniformLocation(const std::string& name)
{
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
    {
        return uniformLocationCache[name];
    }
    else
    {
        uniformLocationCache[name] = glGetUniformLocation(id, name.c_str());
    }

    if (uniformLocationCache[name] == -1) {
        std::cout << "uniform \"" << name << "\" does not exist\n";
    }

    return uniformLocationCache[name];
}

void shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    bind();
    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void shader::setUniform1f(const std::string& name, float v0)
{
    bind();
    glUniform1f(getUniformLocation(name), v0);
}

void shader::setUniform3f(const std::string& name, float v0, float v1, float v2)
{
    bind();
    glUniform3f(getUniformLocation(name), v0, v1, v2);
}

void shader::setUniform1i(const std::string& name, int v0)
{
    bind();
    glUniform1i(getUniformLocation(name), v0);
}

void shader::setUniformMat4fv(const std::string& name, const float* v)
{
    bind();
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, v);
}

void shader::setUniformMat3fv(const std::string& name, const float* v)
{
    bind();
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, v);
}