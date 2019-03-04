#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void processInput(Renderer &r, GLFWwindow *window);

int main()
{
	Renderer r;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(Renderer::window_width, Renderer::window_height, "GloriousBachelorsThesis", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	r.Construct();

	while (!glfwWindowShouldClose(window))
	{
		processInput(r, window);

		r.Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(Renderer &r, GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		r.SetViewMode(Default);

	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		r.SetViewMode(Wireframe);
}