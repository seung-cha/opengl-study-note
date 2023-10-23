#include "Texture.h"

Texture::Texture(const char* source, GLint internalFormat, GLenum format)
{
	int x, y, channel;
	unsigned char* src = stbi_load(source, &x, &y, &channel, 0);

	if (!source)
	{
		std::cout << "Failed to load texture source:" << std::endl;
		std::cout << "\t" << source << std::endl;
		return;

	}


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, format, GL_UNSIGNED_BYTE, src);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free((void*)src);

	glBindTexture(GL_TEXTURE_2D, 0);
}
