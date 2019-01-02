#include "Renderer.h"
#include "Box.h"
#include <glbinding/gl/gl.h>

using namespace gl;

Renderer::Renderer()
{

}

void Renderer::Construct()
{
	test_box_ = CreateObject<Box>(glm::vec3(10.f, 0.f, 0.f), glm::vec3(45.f, 45.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	CreateObject<Box>(glm::vec3(10.f, 5.f, 0.f), glm::vec3(45.f, 45.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	CreateObject<Box>(glm::vec3(10.f, -5.f, 0.f), glm::vec3(45.f, 45.f, 0.f), glm::vec3(2.f, 1.f, 1.f));
	// Model* model = CreateObject<Model>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
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
			dynamic_cast<Object*>(t)->Draw();
		}
	}

	
}