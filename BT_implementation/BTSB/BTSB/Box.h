#pragma once

#include "Object.h"
#include "Shader.h"

class Box : public Object
{
public:

	Box();
	~Box();

	virtual void Draw() override;

private:

	unsigned int vao_;
	unsigned int vbo_;

	Shader box_shader_ = Shader("Shaders/box_shader.vs", "Shaders/box_shader.fs");


};