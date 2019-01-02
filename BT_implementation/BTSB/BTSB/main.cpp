#include "Renderer.h"


int main()
{
	Renderer r;
	glw::Window* window;

	// specify initial window parameters
	glw::Window::InitParameters params;
	params.aa_samples = 0;
	params.try_share_contexts = false;
	params.resolution = glm::uvec2(r.window_width, r.window_height);

	// create new window with parameters
	window = new glw::Window(params);

	// register input_control to receive events from window
	window->ObserverRegister(&r);

	r.Construct();

	while (!window->ShouldClose())
	{
		r.Draw();

		// after drawing swap buffers
		window->PollEvents();

		// inform all window observers about actions
		window->SwapBuffers();
	}

	delete window;

	return 0;
}