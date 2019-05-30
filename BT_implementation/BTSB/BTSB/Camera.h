#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Object.h"
#include <glm/detail/type_vec.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera : public Transform
{


public:
	
	Camera();

	glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 Right;
	glm::vec3 WorldUp = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 WorldFront = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 Target = glm::vec3(0.f, 0.f, 0.f);

	float MovementSpeed = 0.05f;
	float MouseSensitivity = 0.6f;
	float Zoom = 30.0f;
	float TargetZoom = 30.f;

	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset);
	void ProcessMouseScroll(float yoffset);
	void SetOrbit(bool orbit);
	void UpdateCamera();

private:
	bool orbit_camera_ = false;

	void updateCameraVectors();
};
