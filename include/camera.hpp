
#ifndef _GLIMPLIFY_CAMERA_H_
#define _GLIMPLIFY_CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace glimplify {

	/*
	* 
	* The camera system we introduced is a fly like camera that suits most purposes and works well with Euler angles, 
	* but be careful when creating different camera systems like an FPS camera, or a flight simulation camera. 
	* Each camera system has its own tricks and quirks so be sure to read up on them. 
	* For example, this fly camera doesn't allow for pitch values higher than or equal to 90 degrees and 
	* a static up vector of (0,1,0) doesn't work when we take roll values into account.
	* 
	*/

	class camera
	{
		float _width;
		float _height;

		glm::vec3 _position;
		glm::vec3 _front;
		glm::vec3 _up;

		glm::mat4 _view;

		float _move_sensitive;

		float _pitch;
		float _yaw;
		float _rotate_sensitive;

		float _fov;
		float _nearest;
		float _farest;
		glm::mat4 _perspective;

	public:
		explicit camera(int width, int height)
			: _width(width), _height(height)
			, _position(), _front(0.0f, 0.0f, -1.0f), _up(0.0f, 1.0f, 0.0f)
			, _view(glm::lookAt(_position, _position + _front, _up))
			, _move_sensitive(2.5f)
			// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
			, _pitch(0.0f), _yaw(-90.0f), _rotate_sensitive(0.1f)
			, _fov(45.0f), _nearest(0.1f), _farest(100.0f)
			, _perspective(glm::perspective(glm::radians(_fov), _width / _height, _nearest, _farest))
		{
		}

		void move_to(const glm::vec3& position)
		{
			_position = position;
			_view = glm::lookAt(_position, _position + _front, _up);
		}

		void perspective(float fov, float nearest, float farest)
		{
			_fov = fov;
			if (_fov < 1.0f)
			{
				_fov = 1.0f;
			}
			else if (_fov > 45.0f)
			{
				_fov = 45.0f;
			}

			_nearest = nearest;
			_farest = farest;

			_perspective = glm::perspective(glm::radians(_fov), _width / _height, _nearest, _farest);
		}

		void zoom(float fov_offset)
		{
			_fov += fov_offset;

			if (_fov < 1.0f)
			{
				_fov = 1.0f;
			}
			else if (_fov > 45.0f)
			{
				_fov = 45.0f;
			}

			_perspective = glm::perspective(glm::radians(_fov), _width / _height, _nearest, _farest);
		}

		const glm::mat4 forward(float delta_time)
		{
			_position += _move_sensitive * delta_time * _front;
			_view = glm::lookAt(_position, _position + _front, _up);
			return _view;
		}
		
		const glm::mat4 backward(float delta_time)
		{
			_position -= _move_sensitive * delta_time * _front;
			_view = glm::lookAt(_position, _position + _front, _up);
			return _view;
		}

		const glm::mat4 rotate(float pitch_offset, float yaw_offset)
		{
			_pitch += pitch_offset;
			// make sure that when pitch is out of bounds, screen doesn't get flipped
			if (_pitch > 89.0f)
			{
				_pitch = 89.0f;
			}
			else if (_pitch < -89.0f)
			{
				_pitch = -89.0f;
			}

			_yaw += yaw_offset;

			_front = glm::normalize(glm::vec3(cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)), sin(glm::radians(_pitch)), sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))));
			
			_view = glm::lookAt(_position, _position + _front, _up);
			
			return _view;
		}

		const glm::mat4 left(float delta_time)
		{
			glm::vec3 offset = glm::normalize(glm::cross(_front, _up)) * _move_sensitive * delta_time;

			_position -= offset;

			_view = glm::lookAt(_position, _position + _front, _up);

			return _view;
		}

		const glm::mat4 right(float delta_time)
		{
			glm::vec3 offset = glm::normalize(glm::cross(_front, _up)) * _move_sensitive * delta_time;

			_position += offset;

			_view = glm::lookAt(_position, _position + _front, _up);

			return _view;
		}

		const glm::mat4 view_matrix()
		{
			return _view;
		}

		const glm::mat4 perspective_matrix()
		{
			return _perspective;
		}

		~camera()
		{
		}

	private:
		camera() = delete;
		camera(const camera&) = delete;
		camera& operator=(const camera&) = delete;
		camera(camera&&) = delete;
		camera&& operator=(camera&&) = delete;
	};
};

#endif
