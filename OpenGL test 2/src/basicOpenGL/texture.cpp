#include "texture.h"
#include "GL/glew.h"

#include "../external/stb_image/stb_image.h"

texture::texture(const std::string& pathColor, const std::string& pathNormal)
	: path(path), dataBuffer(nullptr), NormalID(0xffffffff)
{
	stbi_set_flip_vertically_on_load(1);
	dataBuffer = stbi_load(pathColor.c_str(), &colorWidth, &colorHeight, &colorBitPerPixel, 4);

	glGenTextures(1, &ColorID);
	glBindTexture(GL_TEXTURE_2D, ColorID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, colorWidth, colorHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (dataBuffer)
	{
		stbi_image_free(dataBuffer);
	}

	if (pathNormal.length() > 0)
	{
		dataBuffer = stbi_load(pathNormal.c_str(), &normalWidth, &normalHeight, &normalBitPerPixel, 4);

		glGenTextures(1, &NormalID);
		glBindTexture(GL_TEXTURE_2D, NormalID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, normalWidth, normalHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (dataBuffer)
		{
			stbi_image_free(dataBuffer);
		}
	}
}

texture::~texture()
{
	glDeleteTextures(1, &ColorID);
	if (!(NormalID == 0xffffffff))
	{
		glDeleteTextures(1, &NormalID);
	}
}

void texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, ColorID);
	if (!(NormalID == 0xffffffff))
	{
		glActiveTexture(GL_TEXTURE0 + slot + 1);
		glBindTexture(GL_TEXTURE_2D, NormalID);
	}
}

void texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
