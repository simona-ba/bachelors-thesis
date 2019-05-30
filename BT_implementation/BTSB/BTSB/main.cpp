#include "Renderer.h"
#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

float deltaTime = 0.01f;	// time between current frame and last frame

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(Renderer &r, GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
	Renderer& r = Renderer::instance();
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(Renderer::window_width, Renderer::window_height, "BachelorsThesis", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	r.Construct();

	// timing
	float lastDeltaTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		deltaTime = glfwGetTime() - lastDeltaTime;
		lastDeltaTime = deltaTime;

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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		r.GetCamera()->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		r.GetCamera()->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		r.GetCamera()->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		r.GetCamera()->ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		r.GetCamera()->SetOrbit(true);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		r.GetCamera()->SetOrbit(false);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Renderer& r = Renderer::instance();
	r.OnWindowResized(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Renderer& r = Renderer::instance();
	r.OnMouseMove(xpos, ypos);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Renderer& r = Renderer::instance();
	r.OnMouseScroll(xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Renderer& r = Renderer::instance();
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		r.ToggleSpecular();
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		r.ToggleBlinn();
	}
	else if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		r.SetViewMode(DEFAULT);
	}
	else if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
	{
		r.SetViewMode(WIREFRAME);
	}
	else if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
	{
		r.SetViewMode(ALBEDO);
	}
	else if (key == GLFW_KEY_F4 && action == GLFW_PRESS)
	{
		r.SetViewMode(UV);
	}	
	else if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
	{
		r.SetViewMode(LIGHTING_ONLY);
	}
	else if (key == GLFW_KEY_F6 && action == GLFW_PRESS)
	{
		r.SetViewMode(NORMALS);
	}
	else if (key == GLFW_KEY_F7 && action == GLFW_PRESS)
	{
		r.SetViewMode(SPECULAR_MAP);
	}
	else if (key == GLFW_KEY_F8 && action == GLFW_PRESS)
	{
		r.SetViewMode(NORMAL_MAP);
	}
}