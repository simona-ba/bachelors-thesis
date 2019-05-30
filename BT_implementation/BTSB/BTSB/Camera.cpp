#include "Camera.h"

Camera::Camera()
{
	UpdateCamera();
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(location, location + Front, Up);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		location += Front * velocity;
	if (direction == BACKWARD)
		location -= Front * velocity;
	if (direction == LEFT)
		location -= Right * velocity;
	if (direction == RIGHT)
		location += Right * velocity;
}

void Camera::SetOrbit(bool orbit)
{
	orbit_camera_ = orbit;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
	if (orbit_camera_ == true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		//rotation.x -= yoffset;
		rotation.x = glm::clamp(rotation.x + yoffset, -85.f, 85.f);
		rotation.y = glm::mod(rotation.y + xoffset, 360.f);
		rotation.z = 0.f;

		UpdateCamera();
	}
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
	Zoom -= yoffset;
	UpdateCamera();
}

	// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	glm::mat4 trans = glm::mat4(1.f);

	trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f)); // Otocenie okolo X (hore, dole)
	trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f)); // Otocenie okolo Y (dolava, doprava)
	Front = glm::vec3(glm::vec4(WorldFront, 0.f) * trans);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::UpdateCamera()
{
	
	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();

	location = Target + Front * -Zoom;
}