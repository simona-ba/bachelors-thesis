#pragma once

#include "Object.h"
#include "Shader.h"
#include "vector"
#include "string"

class Cubemap : public Object
{
public:

	Cubemap();
	~Cubemap();

	virtual void Draw(Shader shader) override;

private:

	unsigned int vao_;
	unsigned int vbo_;
	Shader* cubemap_shader_;
	unsigned cubemap_texture_;
	
	unsigned int loadCubemap(std::vector<std::string> faces);
	

};