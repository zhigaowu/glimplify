
#ifndef _GLIMPLIFY_PROGRAM_H_
#define _GLIMPLIFY_PROGRAM_H_

#include "size_of.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>

namespace glimplify {

	class program
	{
		class shader
		{
			GLenum _type;
			GLuint _id;

		public:
			explicit shader(GLenum type, const char* source)
				: _type(type), _id(glCreateShader(type))
			{
				glShaderSource(_id, 1, &source, NULL);
				glCompileShader(_id);
			}

			bool check(GLsizei length, GLchar* desc)
			{
				// check for shader compile errors
				int status = 0;
				glGetShaderiv(_id, GL_COMPILE_STATUS, &status);
				if (0 == status)
				{
					glGetShaderInfoLog(_id, length, NULL, desc);
				}
				return 0 != status;
			}

			~shader()
			{
				glDeleteShader(_id);
			}

			friend class program;

		private:
			shader() = delete;
			shader(const shader&) = delete;
			shader& operator=(const shader&) = delete;
			shader(shader&&) = delete;
			shader&& operator=(shader&&) = delete;
		};

		using uniform_name_loactions = std::unordered_map<std::string, GLint>;

		GLuint _id;
		uniform_name_loactions _uniform_name_loactions;

		GLint uniform_location(const char* name)
		{
			GLint location = -1;
			uniform_name_loactions::iterator name_location = _uniform_name_loactions.find(name);
			if (_uniform_name_loactions.end() != name_location)
			{
				location = name_location->second;
			}
			else
			{
				location = glGetUniformLocation(_id, name);

				_uniform_name_loactions.insert(std::make_pair(name, location));
			}

			return location;
		}

	public:
		program()
			: _id(glCreateProgram())
		{
		}

		bool compile(const char* vertex_shader_source, const char* fragment_shader_source, GLsizei length, GLchar* desc)
		{
			shader vertex_shader(GL_VERTEX_SHADER, vertex_shader_source);
			shader fragment_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

			bool res = vertex_shader.check(length, desc) && fragment_shader.check(length, desc);

			if (res)
			{
				glAttachShader(_id, vertex_shader._id);
				glAttachShader(_id, fragment_shader._id);
				glLinkProgram(_id);

				int status = 0;
				glGetProgramiv(_id, GL_LINK_STATUS, &status);
				if (0 == status) 
				{
					glGetProgramInfoLog(_id, length, NULL, desc);
				}

				res = (0 != status);
			}

			return res;
		}

		void bind()
		{
			glUseProgram(_id);
		}

		void set_uniform_1i(const char* name, GLint value)
		{
			glUniform1i(uniform_location(name), value);
		}

		void set_uniform_matrix4fv(const char* name, const glm::mat4& value)
		{
			glUniformMatrix4fv(uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		void unbind()
		{
			glUseProgram(0);
		}

		~program()
		{
			glDeleteProgram(_id);
		}

	private:
		program(const program&) = delete;
		program& operator=(const program&) = delete;
		program(program&&) = delete;
		program&& operator=(program&&) = delete;
	};
};

#endif
