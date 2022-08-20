
#ifndef _GLIMPLIFY_SIZE_OF_H_
#define _GLIMPLIFY_SIZE_OF_H_

#include <glad/glad.h>

namespace glimplify {

	GLsizei size_of(GLenum type)
	{
		static const size_t sizes[7] = { sizeof(GLbyte), sizeof(GLubyte), sizeof(GLshort), sizeof(GLushort), sizeof(GLint), sizeof(GLuint), sizeof(GLfloat) };
		return static_cast<GLsizei>(sizes[type - GL_BYTE]);
	}
};

#endif
