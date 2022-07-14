#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "PerspectiveCamera.h"
#include "World.h"
#include "Statistics.h"
#include "Player.h"
#include "UserInterface.h"

#include "Renderer.h"

GLFWwindow* InitWindow() {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return nullptr;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1920, 1080, "MC-Clone", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return nullptr;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(0);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error initialising GLEW." << std::endl;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	std::cout << glGetString(GL_VERSION) << std::endl;

	return window;
}

int main(void) {

	GLFWwindow* window = InitWindow();
	if (!window)
		return -1;

	World* world = new World(3, 20);
	PerspectiveCamera* cam = new PerspectiveCamera(window, glm::vec3(0, 100, 0), -80, 0.1f, 1000, 0, 0);
	Renderer renderer(cam, window, world);
	Player player(cam);
	Statistics stats;
	UserInterface ui(window);

	float prevTime = (float)glfwGetTime(), currentTime = (float)glfwGetTime(), dt = 0;

	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		renderer.BeginFrame();

		prevTime = currentTime;
		currentTime = (float)glfwGetTime();
		dt = currentTime - prevTime;

		renderer.RenderFrame(dt);

		stats.Update(dt);
		player.Update(dt, window, world, renderer);

		ui.Update(player);

		renderer.EndFrame();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}