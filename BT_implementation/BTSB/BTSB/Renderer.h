#pragma once
#include <iostream>
#include <memory>
#include <GLW/Window.h>
#include <GLW/Texture.h>
#include <string>
#include "Object.h"
#include "Box.h"

using namespace std;
using namespace glw;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Renderer : public glw::WindowEventObserver
{
public:
	
	static const int window_width = 640;
	static const int window_height = 480;

	Renderer();

	void Construct();

	void Draw();
	
	template<typename T>
	T* CreateObject(glm::vec3 location, glm::vec3 rotation, glm::vec3 scale)
	{
		T* obj = new T;
		obj->SetLocation(location);
		obj->SetRotation(rotation);
		obj->SetScale(scale);
		transforms.push_back(obj);
		return obj;
	}

protected:

	std::vector<Transform*> transforms;

private:

	Box* test_box_;

	Shader* debug_shader_;
};

