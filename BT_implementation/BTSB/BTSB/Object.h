#pragma once
#include <glm/glm.hpp>

class Object
{
public:
	
	void SetLocation(glm::vec3 new_location);
	void SetRotation(glm::vec3 new_rotation);
	void SetScale(glm::vec3 new_scale);

	const glm::vec3& GetLocation() const { return location; }
	const glm::vec3& GetRotation() const { return rotation; }
	const glm::vec3& GetScale() const { return scale; }

	virtual void Draw() {} 

protected:

	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::vec4 model_matrix;

	void UpdateModelMatrix();

private:

	Object() {}

};
