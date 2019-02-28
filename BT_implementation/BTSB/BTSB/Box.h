#pragma once

#include "Object.h"
#include "Shader.h"

class Box : public Object
{
public:

	Box();
	~Box();

	virtual void Draw(Shader shader) override;

private:

	unsigned int vao_;
	unsigned int vbo_;

	


};