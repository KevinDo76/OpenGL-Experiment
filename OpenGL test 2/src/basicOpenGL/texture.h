#pragma once
#include <string>
class texture
{
public:
	texture(const std::string& pathColor, const std::string& pathNormal = "");
	~texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidthColor() const { return colorWidth; }
	inline int getHeightColor() const { return colorHeight; }

private:
	std::string path;
	unsigned int ColorID;
	unsigned int NormalID;
	unsigned char* dataBuffer;
	int colorWidth, colorHeight, colorBitPerPixel;
	int normalWidth, normalHeight, normalBitPerPixel;
};

