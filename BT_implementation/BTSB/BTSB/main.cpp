#include "Renderer.h"
#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

float delta_time = 0.01f;	// time between current frame and last frame

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
void processInput(Renderer &r, GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mods);

int main(int argc, char *argv[])
{
	Renderer& r = Renderer::instance();
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(Renderer::instance().window_width, Renderer::instance().window_height, "BachelorsThesis", NULL, NULL);
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

	if (argc == 2)
	{
		r.SetDefaultModel(argv[1]);
	}

	r.Construct();

	// timing
	float last_delta_time = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		delta_time = glfwGetTime() - last_delta_time;
		last_delta_time = delta_time;

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
		r.GetCamera()->ProcessKeyboard(FORWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		r.GetCamera()->ProcessKeyboard(BACKWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		r.GetCamera()->ProcessKeyboard(LEFT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		r.GetCamera()->ProcessKeyboard(RIGHT, delta_time);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		r.GetCamera()->SetOrbit(true);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		r.GetCamera()->SetOrbit(false);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		r.GetCamera()->SetMove(true);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
		r.GetCamera()->SetMove(false);
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
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	Renderer& r = Renderer::instance();
	r.OnMouseScroll(x_offset, y_offset);
}

void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mods)
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
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		r.ToggleNormal();
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