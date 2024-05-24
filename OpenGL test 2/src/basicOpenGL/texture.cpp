#include "texture.h"
#include "GL/glew.h"

#include "../external/stb_image/stb_image.h"

texture::texture(const std::string& path)
	: path(path), dataBuffer(nullptr)
{
	stbi_set_flip_vertically_on_load(1);
	dataBuffer = stbi_load(path.c_str(), &width, &height, &bitPerPixel, 4);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (dataBuffer)
	{
		stbi_image_free(dataBuffer);
	}
}

texture::~texture()
{
	glDeleteTextures(1, &id);
}

void texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
