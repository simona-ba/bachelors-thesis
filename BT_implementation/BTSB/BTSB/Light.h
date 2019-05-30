#pragma once

#include "Object.h"
#include "Shader.h"

class Light : public Object
{
public:

	Light();
	~Light();

	virtual void Draw(Shader shader) override;

private:

	unsigned int vao_;
	unsigned int vbo_;

	Shader* lamp_;

};
