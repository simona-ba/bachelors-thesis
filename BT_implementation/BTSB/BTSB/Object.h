#pragma once

#include <glm/glm.hpp>
#include "Shader.h"


class Transform
{
public:
	void SetLocation(glm::vec3 new_location);
	void SetRotation(glm::vec3 new_rotation);
	void SetScale(glm::vec3 new_scale);

	const glm::vec3& GetLocation() const { return location; }
	const glm::vec3& GetRotation() const { return rotation; }
	const glm::vec3& GetScale() const { return scale; }

protected:

	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 model_matrix;

	void UpdateModelMatrix();

	virtual ~Transform() {}
};

class Object : public Transform
{
public:

	virtual void Draw(Shader shader) = 0;

};
