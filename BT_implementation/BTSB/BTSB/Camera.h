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

	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right;
	glm::vec3 world_up = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 world_front = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 target = glm::vec3(0.f, 0.f, 0.f);

	float movement_speed = 0.05f;
	float mouse_sensitivity_orbit = 0.6f;
	float mouse_sensitivity_move = 0.1f;
	float zoom = 30.0f;
	float min_zoom = 5.f;
	float max_zoom = 100.f;

	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(Camera_Movement direction, float delta_time);
	void ProcessMouseMovement(float x_offset, float y_offset);
	void ProcessMouseScroll(float y_offset);
	void SetOrbit(bool orbit);
	void SetMove(bool move);
	void UpdateCamera();

private:
	bool orbit_camera_ = false;
	bool move_camera_ = false;

	void updateCameraVectors();
};
