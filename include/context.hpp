
#ifndef _GLIMPLIFY_CONTEXT_H_
#define _GLIMPLIFY_CONTEXT_H_

#include <glad/glad.h>

namespace glimplify {

	class context
	{
		GLbitfield _clear_bitfield;

	public:
		explicit context(GLDEBUGPROC callback, const void* user_data = nullptr)
			: _clear_bitfield(GL_COLOR_BUFFER_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(callback, user_data);
		}

		void wireframe_mode()
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		void testing_depth(bool testing = true)
		{
			if (testing)
			{
				_clear_bitfield = _clear_bitfield | GL_DEPTH_BUFFER_BIT;
				glEnable(GL_DEPTH_TEST);
			}
			else
			{
				_clear_bitfield = _clear_bitfield & (GL_DEPTH_BUFFER_BIT ^ 0xFFFFFFFF);
				glDisable(GL_DEPTH_TEST);
			}
		}

		void clear(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
		{
			glClearColor(red, green, blue, alpha);
			glClear(_clear_bitfield);
		}

		~context()
		{
		}

	private:
		context() = delete;
		context(const context&) = delete;
		context& operator=(const context&) = delete;
		context(context&&) = delete;
		context&& operator=(context&&) = delete;
	};
};

#endif
