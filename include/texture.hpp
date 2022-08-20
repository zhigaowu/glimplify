
#ifndef _GLIMPLIFY_TEXTURE_H_
#define _GLIMPLIFY_TEXTURE_H_

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace glimplify {

	class texture
	{
		GLenum _texture_unit;

		GLuint _id;

		GLint _width;
		GLint _aligned_width;
		GLint _height;
		GLint _channels;

	public:
		texture(GLenum texture_unit = 0)
			: _texture_unit(texture_unit), _id(0)
			, _width(0), _aligned_width(0), _height(0), _channels(0)
		{
			glGenTextures(1, &_id);
		}

		void bind()
		{
			glActiveTexture(GL_TEXTURE0 + _texture_unit);
			glBindTexture(GL_TEXTURE_2D, _id);
		}

		void wrap_mode(GLint s_mode, GLint t_mode)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_mode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_mode);
		}

		void filter_mode(GLint min_mode, GLint mag_mode)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_mode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_mode);
		}

		void load(const char* image_path, bool flip_on_vertical, bool generate_mipmap = false)
		{
			if (flip_on_vertical)
			{
				stbi_set_flip_vertically_on_load(true);
			}

			unsigned char* data = stbi_load(image_path, &_width, &_height, &_channels, 0);

			if (data)
			{
				_aligned_width = _width;

				if (_channels > 3)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				}
				else
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				}

				if (generate_mipmap)
				{
					glGenerateMipmap(GL_TEXTURE_2D);
				}

				stbi_image_free(data);
			}
		}

		void unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE0);
		}

		~texture()
		{
			glDeleteTextures(1, &_id);
		}

	private:
		texture(const texture&) = delete;
		texture& operator=(const texture&) = delete;
		texture(texture&&) = delete;
		texture&& operator=(texture&&) = delete;
	};
};

#endif
