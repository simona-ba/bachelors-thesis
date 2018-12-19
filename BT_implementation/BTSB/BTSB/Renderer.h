#pragma once
#include <iostream>
#include <memory>
#include <GLW/Window.h>
#include "Object.h"

class MyObserver : public glw::WindowEventObserver
{
	// this method is called every time someone clicks on window
	// see implementation of WindowEventObserver for many other methods
	virtual void OnMouseDown(int button, int mods) override
	{
		std::cout << "Mouse key was pressed" << std::endl;
	}
};

class Renderer
{
public:
	
	static const int window_width = 640;
	static const int window_height = 480;

	glw::Window* window;

	Renderer();
	~Renderer();

	void Construct();

	void RenderLoop();
	
	template<typename T>
	T* CreateObject(glm::vec3 location, glm::vec3 rotation, glm::vec3 scale)
	{
		T* obj = new T;
		obj->SetLocation(location);
		obj->SetRotation(rotation);
		obj->SetScale(scale);
		objects.push_back(obj);
		return obj;
	}

protected:

	std::vector<Object*> objects;

};

