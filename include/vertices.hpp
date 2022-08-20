
#ifndef _GLIMPLIFY_VERTICES_H_
#define _GLIMPLIFY_VERTICES_H_

#include "size_of.hpp"

#include <vector>
#include <numeric>

namespace glimplify {

	class vertices
	{
		GLuint _vao;
		GLuint _vbo;
		GLuint _ebo;

		GLsizei _stride;

		std::vector<GLsizei> _attribute_size;

	public:
		explicit vertices(GLuint attributes, GLsizei stride)
			: _vao(0), _vbo(0), _ebo(0)
			, _attribute_size(attributes, 0)
			, _stride(stride)
		{
			glGenVertexArrays(1, &_vao);
			glGenBuffers(1, &_vbo);
		}

		void bind()
		{
			glBindVertexArray(_vao);
		}

		void allocate_vertices(GLsizeiptr size, const void* data, GLenum usage)
		{
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glBufferData(GL_ARRAY_BUFFER, size, data, usage);
		}

		void format_vertices(GLuint index, GLenum type, GLint size)
		{
			_attribute_size[index] = size_of(type) * size;

			glVertexAttribPointer(index, size, type, GL_FALSE, _stride, (const void*)(GLsizeiptr)(std::accumulate(_attribute_size.begin(), _attribute_size.begin() + index, 0)));
			glEnableVertexAttribArray(index);
		}

		void allocate_index(GLsizeiptr size, const void* data, GLenum usage)
		{
			glGenBuffers(1, &_ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
		}

		void unbind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		~vertices()
		{
			glDeleteVertexArrays(1, &_vao);
			glDeleteBuffers(1, &_vbo);

			if (_ebo > 0)
			{
				glDeleteBuffers(1, &_ebo);
			}
		}

	private:
		vertices() = delete;
		vertices(const vertices&) = delete;
		vertices& operator=(const vertices&) = delete;
		vertices(vertices&&) = delete;
		vertices&& operator=(vertices&&) = delete;
	};
};

#endif
