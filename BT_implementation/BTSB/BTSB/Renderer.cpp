#include "Renderer.h"
#include "Box.h"
#include "Model.h"
#include <glbinding/gl/gl.h>

using namespace gl;

Renderer::Renderer()
{

}

void Renderer::Construct()
{
	debug_shader_ = new Shader("Shaders/nano.vs", "Shaders/nano.fs");

	//test_box_ = CreateObject<Box>(glm::vec3(10.f, 0.f, 0.f), glm::vec3(45.f, 45.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	//CreateObject<Box>(glm::vec3(10.f, 5.f, 0.f), glm::vec3(45.f, 45.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	//CreateObject<Box>(glm::vec3(10.f, -5.f, 0.f), glm::vec3(45.f, 45.f, 0.f), glm::vec3(2.f, 1.f, 1.f));

	Model* model = CreateObject<Model>(glm::vec3(10.f, 5.f, 0.f), glm::vec3(45.f, 45.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	model->loadModel(string("Resources/nanosuit/nanosuit.obj"));

	/*model = CreateObject<Model>(glm::vec3(20.f, 5.f, -5.f), glm::vec3(45.f, 45.f, 0.f), glm::vec3(1.f, 1.f, 101.f));
	model->loadModel(string("Resources/nanosuit/nanosuit.obj"));

	model = CreateObject<Model>(glm::vec3(10.f, -5.f, 0.f), glm::vec3(45.f, 45.f, 0.f), glm::vec3(1.f, 5.f, 1.f));
	model->loadModel(string("Resources/nanosuit/nanosuit.obj"));

	model = CreateObject<Model>(glm::vec3(10.f, -50.f, 0.f), glm::vec3(45.f, 45.f, 0.f), glm::vec3(10.f, 10.f, 10.f));
	model->loadModel(string("Resources/nanosuit/nanosuit.obj"));*/
	// model-> ...
}

void Renderer::Draw()
{	
	glClearColor(0.f, 0.1f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

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

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	PTexture2D texture = Texture2D::CreateFromFile(filename);

	texture->Bind();
	texture->SetWrapping(Wrapping::REPEAT);
	texture->SetFiltering(Filtering::LINEAR, Filtering::LINEAR_INTER_MIP);

	return texture->GetID();
}
