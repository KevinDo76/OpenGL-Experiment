//molasses objparser(molasses is probably faster)
#pragma once
#include <string>
#include <vector>
namespace objParser {
	void parse(std::string filePath, std::vector<float>& vertexData, std::vector<unsigned int>& indices);
}