#pragma once
#include <string>
class texture
{
public:
	texture(const std::string& path);
	~texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }

private:
	std::string path;
	unsigned int id;
	unsigned char* dataBuffer;
	int width, height, bitPerPixel;
};

