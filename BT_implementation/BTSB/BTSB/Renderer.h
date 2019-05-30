#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "Object.h"
#include "Box.h"
#include "Camera.h"

using namespace std;

enum ViewMode
{
	DEFAULT,
	WIREFRAME,
	ALBEDO,
	UV,
	LIGHTING_ONLY,
	NORMALS,
	SPECULAR_MAP,
	NORMAL_MAP
};

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Renderer
{
public:
	
	static const int window_width = 640 * 2;
	static const int window_height = 480 * 2;
	ViewMode currentViewMode = DEFAULT;

	static Renderer& instance(); 

	void Construct();

	void Draw();

	void SetViewMode(ViewMode mode);
	
	template<typename T>
	T* CreateObject(glm::vec3 location, glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f))
	{
		T* obj = new T;
		obj->SetLocation(location);
		obj->SetRotation(rotation);
		obj->SetScale(scale);
		transforms.push_back(obj);
		return obj;
	}

	void OnWindowResized(int new_width, int new_height);

	void OnMouseMove(double x, double y);

	void OnMouseScroll(double xoffset, double yoffset);

	Camera* GetCamera();

	void ToggleSpecular();

	bool IsSpecularEnabled();

	void ToggleBlinn();

	bool IsBlinnEnabled();

protected:

	std::vector<Transform*> transforms;

private:

	Box* test_box_;

	Shader* debug_shader_;

	Camera* camera_;

	Renderer() {}
	
	bool specularEnabled_ = true;
	bool blinnEnabled_ = true;
	bool firstMouse_ = true;
	float lastX_;
	float lastY_;
};

