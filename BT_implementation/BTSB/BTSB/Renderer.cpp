#include "Renderer.h"
#include "Box.h"
#include "Model.h"
#include <glad/glad.h> 
#include <stb_image/stb_image.h>


Renderer::Renderer()
{

}

void Renderer::Construct()
{
	glEnable(GL_DEPTH_TEST);

	debug_shader_ = new Shader("Shaders/nano.vs", "Shaders/nano.fs");

	test_box_ = CreateObject<Box>(glm::vec3(10.f, 0.f, 0.f), glm::vec3(45.f, 45.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	CreateObject<Box>(glm::vec3(10.f, 5.f, 0.f), glm::vec3(45.f, 45.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	//CreateObject<Box>(glm::vec3(10.f, -5.f, 0.f), glm::vec3(45.f, 45.f, 0.f), glm::vec3(2.f, 1.f, 1.f));

	Model* model = CreateObject<Model>(glm::vec3(0.0f, -1.75f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.2f, 0.2f, 0.2f));
	model->loadModel(string("Resources/nanosuit/nanosuit.obj"));

	model = CreateObject<Model>(glm::vec3(20.f, 5.f, -5.f), glm::vec3(45.f, 45.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	model->loadModel(string("Resources/nanosuit/nanosuit.obj"));


	// model-> ...
}

void Renderer::Draw()
{	
	switch (currentViewMode)
	{
	case Default:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case Wireframe:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	default:
		break;
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
	currentViewMode = mode;
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
