#include "Object.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

void Transform::SetLocation(glm::vec3 new_location)
{
	location = new_location;
	UpdateModelMatrix();
}

void Transform::SetRotation(glm::vec3 new_rotation)
{
	rotation = new_rotation;
	UpdateModelMatrix();
}

void Transform::SetScale(glm::vec3 new_scale)
{
	scale = new_scale;
	UpdateModelMatrix();
}

void Transform::UpdateModelMatrix()
{
	// calculate the model matrix for each object and pass it to shader before drawing
	model_matrix = glm::mat4();
	model_matrix = glm::translate(model_matrix, GetLocation());
	model_matrix = glm::rotate(model_matrix, glm::radians(GetRotation().x), glm::vec3(1.f, 0.f, 0.f));
	model_matrix = glm::rotate(model_matrix, glm::radians(GetRotation().y), glm::vec3(0.f, 1.f, 0.f));
	model_matrix = glm::rotate(model_matrix, glm::radians(GetRotation().z), glm::vec3(0.f, 0.f, 1.f));
	model_matrix = glm::scale(model_matrix, GetScale());
}