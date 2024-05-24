//molasses objparser(molasses is probably faster)
#include "objParser.h"
#include <fstream>
#include <array>
#include <sstream>
#include <iostream>

//prob should replace with assimp later
static void split(std::string& s, const std::string& delimiter, std::vector<std::string>& result)
{
	result.reserve(4);
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		result.emplace_back(token);
		s.erase(0, pos + delimiter.length());
	}
	result.emplace_back(s);
}

static void processVertex(std::string& vertexString, std::vector<std::array<float, 3>>& vertexPosition, std::vector<std::array<float, 2>>& vertexUV, std::vector<std::array<float, 3>>& vertexNormal, std::vector<float>& vertexData)
{
	std::vector<std::string> result;
	split(vertexString, "/", result);
	//push in positional data
	vertexData.emplace_back(vertexPosition[std::atoi(result[0].c_str()) - 1][0]);
	vertexData.emplace_back(vertexPosition[std::atoi(result[0].c_str()) - 1][1]);
	vertexData.emplace_back(vertexPosition[std::atoi(result[0].c_str()) - 1][2]);
	if (result.size() > 1 && result[1].size()>0)
	{
		vertexData.emplace_back(vertexUV[std::atoi(result[1].c_str()) - 1][0]);
		vertexData.emplace_back(vertexUV[std::atoi(result[1].c_str()) - 1][1]);
	}
	else
	{
		vertexData.emplace_back(0);
		vertexData.emplace_back(0);
	}
	if (result.size() > 2 && result[2].size() > 0)
	{
		vertexData.emplace_back(vertexNormal[std::atoi(result[2].c_str()) - 1][0]);
		vertexData.emplace_back(vertexNormal[std::atoi(result[2].c_str()) - 1][1]);
		vertexData.emplace_back(vertexNormal[std::atoi(result[2].c_str()) - 1][2]);
	}
	else
	{
		vertexData.emplace_back(0);
		vertexData.emplace_back(0);
		vertexData.emplace_back(0);
	}
}

void objParser::parse(std::string filePath, std::vector<float>& vertexData, std::vector<unsigned int>& indices)
{
	std::cout << "Parse Start\n";
	std::ifstream fileHandle(filePath.c_str());
	std::string line;
	bool vertexFinished = false;

	std::vector<std::array<float, 3>> vertexPosition;
	std::vector<std::array<float, 2>> vertexUV;
	std::vector<std::array<float, 3>> vertexNormal;
	if (!fileHandle.is_open()) {
		std::cout << "Failed to read file \"" << filePath << "\"\n";
		return;
	}

	int vertexCount = 0;
	int UVCount = 0;
	int NormalCount = 0;
	int triCount = 0;

	std::vector<std::string> data;

	std::cout << "start\n";
	/*while (getline(fileHandle, line))
	{
		if (line[0] == 'v' && line[1] == ' ')
		{
			vertexCount++;
			data.emplace_back(line+'\n');
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			NormalCount++;
			data.emplace_back(line + '\n');
		}
		else if (line[0] == 'v' && line[1] == 't')
		{
			UVCount++;
			data.emplace_back(line + '\n');
		}
		else if (line[0] == 'f')
		{
			triCount++;
			data.emplace_back(line + '\n');
		}
	}
	std::cout << "done\n";

	vertexPosition.reserve(vertexCount);
	vertexUV.reserve(UVCount);
	vertexNormal.reserve(NormalCount);
	vertexData.reserve(triCount * 8 * 4);
	*/
	//for (int i=0;i<data.size();i++)
	while (getline(fileHandle, line))
	{
		//std::string line = data[i];
		if (line[0] == 'v' && line[1] == ' ')
		{
			std::vector<std::string> result;

			split(line, " ", result);

			vertexPosition.push_back({ std::stof(result[1]), std::stof(result[2]), std::stof(result[3])});
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			std::vector<std::string> result;

			split(line, " ", result);
			vertexNormal.push_back({ std::stof(result[1]), std::stof(result[2]), std::stof(result[3])});
		}
		else if (line[0] == 'v' && line[1] == 't')
		{

			std::vector<std::string> result;

			split(line, " ", result);

			vertexUV.push_back({ std::stof(result[1]), std::stof(result[2])});
		}
		//todo, actually optimize the indices
		else if (line[0] == 'f')
		{
			if (!vertexFinished)
			{
				vertexFinished = true;
				vertexData.reserve(vertexPosition.size() * 8 * 3);
				indices.reserve(vertexPosition.size());
				std::cout << vertexPosition.size() * 8 * 3 << "\n";
			}

			std::vector<std::string> result;

			split(line, " ", result);

			processVertex(result[1], vertexPosition, vertexUV, vertexNormal, vertexData);
			processVertex(result[2], vertexPosition, vertexUV, vertexNormal, vertexData);
			processVertex(result[3], vertexPosition, vertexUV, vertexNormal, vertexData);
			indices.emplace_back(indices.size());
			indices.emplace_back(indices.size());
			indices.emplace_back(indices.size());
		}
	}
	std::cout << "Parse End\n";
	vertexPosition.clear();
	vertexUV.clear();
	vertexNormal.clear();
}
