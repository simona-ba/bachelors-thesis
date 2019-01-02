#include "Renderer.h"
#include <glbinding/gl/gl.h>

using namespace gl;

Renderer::Renderer()
{
	// specify initial window parameters
	glw::Window::InitParameters params;
	params.aa_samples = 0;
	params.try_share_contexts = false;
	params.resolution = glm::uvec2(window_width, window_height);

	// create new window with parameters
	window = new glw::Window(params);

	

	// register input_control to receive events from window
	window->ObserverRegister(this);

	Construct();

	RenderLoop();
}

Renderer::~Renderer()
{
	delete window;
}

void Renderer::Construct()
{
	// Model* model = CreateObject<Model>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	// model-> ...
}

void Renderer::RenderLoop()
{
	// repeat until close button on window is pressed
	while (!window->ShouldClose())
	{
		glClearColor(0.f, 0.1f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// DRAWING HAPPENS HERE
		for (auto& o : objects)
		{
			o->Draw();
		}

		// after drawing swap buffers
		window->PollEvents();

		// inform all window observers about actions
		window->SwapBuffers();
	}
}