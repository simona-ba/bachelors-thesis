#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "Object.h"
#include "Box.h"
#include "Camera.h"
#include "Light.h"

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
	
	int window_width = 640;
	int window_height = 480;
	ViewMode current_view_mode = DEFAULT;

	static Renderer& instance(); 

	void Construct();

	void Draw();

	void SetViewMode(ViewMode mode);
	
	template<typename T>
	T* CreateObject(glm::vec3 location = glm::vec3(0.f, 0.f, 0.f), glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f), glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f))
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

	void OnMouseScroll(double x_offset, double y_offset);

	Camera* GetCamera();

	Light* GetLight();

	void ToggleSpecular();

	bool IsSpecularEnabled();

	void ToggleBlinn();

	bool IsBlinnEnabled();

	void ToggleNormal();

	bool IsNormalEnabled();

	void SetDefaultModel(std::string path);

protected:

	std::vector<Transform*> transforms;

private:

	Box* test_box_;

	Shader* debug_shader_;

	Camera* camera_;

	Light* light_;

	std::string default_model_path_;

	Renderer() {}
	
	bool specular_enabled_ = true;
	bool blinn_enabled_ = true;
	bool normal_enabled_ = true;
	bool first_mouse_ = true;
	float lastX_;
	float lastY_;
};

