#pragma once
#include <string>
#include <unordered_map>
#include "light.h"
#include <vector>
class shader
{
public:
	shader(std::string vertexPath, std::string fragPath, int lightPointC, int lightDirC, std::string geoPath="");
	~shader();

	void bind() const;
	void unbind() const;

	int getUniformLocation(const std::string& name);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniform1f(const std::string& name, float v0);
	void setUniform3f(const std::string& name, float v0, float v1, float v2);
	void setUniform1i(const std::string& name, int v0);
	void setUniformMat4fv(const std::string& name, const float* v);
	void setUniformMat3fv(const std::string& name, const float* v);

	std::vector<lightPoint> lightPointArray;
	std::vector<lighDir> lightDirArray;
private:
	const unsigned int lightPointCount;
	const unsigned int lightDirCount;
	unsigned int id;
	std::unordered_map<std::string, int>uniformLocationCache;
};

