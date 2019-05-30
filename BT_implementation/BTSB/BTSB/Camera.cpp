#include "Camera.h"

Camera::Camera()
{
	UpdateCamera();
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(location, location + front, up);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float delta_time)
{
	float velocity = movement_speed * delta_time;
	if (direction == FORWARD)
		location += front * velocity;
	if (direction == BACKWARD)
		location -= front * velocity;
	if (direction == LEFT)
		location -= right * velocity;
	if (direction == RIGHT)
		location += right * velocity;
}

void Camera::SetOrbit(bool orbit)
{
	orbit_camera_ = orbit;
}

void Camera::SetMove(bool move)
{
	move_camera_ = move;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float x_offset, float y_offset)
{
	if (orbit_camera_ == true)
	{
		x_offset *= mouse_sensitivity_orbit;
		y_offset *= mouse_sensitivity_orbit;

		//rotation.x -= yoffset;
		rotation.x = glm::clamp(rotation.x + y_offset, -85.f, 85.f);
		rotation.y = glm::mod(rotation.y + x_offset, 360.f);
		rotation.z = 0.f;
	}
	else if (move_camera_ == true)
	{
		x_offset *= mouse_sensitivity_move * -1;
		y_offset *= mouse_sensitivity_move;

		target += (right * glm::vec3(1.f, 0.f, 1.f)) * x_offset;
		target += (front * glm::vec3(1.f, 0.f, 1.f)) * y_offset;
	}

	UpdateCamera();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float y_offset)
{
	zoom = glm::clamp<float>(zoom - y_offset, min_zoom, max_zoom);
	UpdateCamera();
}

	// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::mat4 trans = glm::mat4(1.f);

	trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f)); // Otocenie okolo X (hore, dole)
	trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f)); // Otocenie okolo Y (dolava, doprava)
	front = glm::vec3(glm::vec4(world_front, 0.f) * trans);
	// Also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, world_up));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}

void Camera::UpdateCamera()
{
	
	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();

	location = target + front * -zoom;
}