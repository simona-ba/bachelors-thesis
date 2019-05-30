#include "Renderer.h"
#include "Box.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include <glad/glad.h> 
#include <stb_image/stb_image.h>
#include "Cubemap.h"



Renderer& Renderer::instance()
{
	static Renderer r;
	return r;
}

void Renderer::Construct()
{
	glEnable(GL_DEPTH_TEST);

	debug_shader_ = new Shader("shaders/main.vs", "shaders/main.fs");

	camera_ = CreateObject<Camera>();
	camera_->UpdateCamera();

	CreateObject<Cubemap>();

	Model* model = CreateObject<Model>(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.2f, 0.2f, 0.2f));

	if (default_model_path_ == "")
	{
		model->loadModel(string("resources/materialball/export3dcoat.obj"));
	}
	else
	{
		model->loadModel(string("resources/") + default_model_path_);
	}

	light_ = CreateObject<Light>(glm::vec3(2.f, 3.f, 5.f));
}

void Renderer::Draw()
{	
	if (current_view_mode == WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glClearColor(0.f, 0.1f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (test_box_)
	{
		test_box_->SetRotation(test_box_->GetRotation() + glm::vec3(0.001f, 0.f, 0.f));
	}

	// DRAWING HAPPENS HERE
	for (auto& t : transforms)
	{
		if (dynamic_cast<Object*>(t))
		{
			dynamic_cast<Object*>(t)->Draw(*debug_shader_);
		}
	}
}

void Renderer::SetViewMode(ViewMode mode)
{
	current_view_mode = mode;
}

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void Renderer::OnMouseMove(double x, double y)
{
	if (first_mouse_)
	{
		lastX_ = x;
		lastY_ = y;
		first_mouse_ = false;
	}

	float xoffset = x - lastX_;
	float yoffset = y - lastY_;

	lastX_ = x;
	lastY_ = y;

	camera_->ProcessMouseMovement(xoffset, yoffset);
}


void Renderer::OnMouseScroll(double xoffset, double yoffset)
{
	camera_->ProcessMouseScroll(yoffset);
}

void Renderer::OnWindowResized(int new_width, int new_height)
{
	glViewport(0, 0, new_width, new_height);
	window_width = new_width;
	window_height = new_height;
}

Camera* Renderer::GetCamera()
{
	return camera_;
}

Light* Renderer::GetLight()
{
	return light_;
}

void Renderer::ToggleSpecular()
{
	specular_enabled_ = !specular_enabled_;
}

bool Renderer::IsSpecularEnabled()
{
	return specular_enabled_;
}

void Renderer::ToggleBlinn()
{
	blinn_enabled_ = !blinn_enabled_;
}

bool Renderer::IsBlinnEnabled()
{
	return blinn_enabled_;
}

void Renderer::ToggleNormal()
{
	normal_enabled_ = !normal_enabled_;
}

bool Renderer::IsNormalEnabled()
{
	return normal_enabled_;
}

void Renderer::SetDefaultModel(std::string path)
{
	default_model_path_ = path;
}
