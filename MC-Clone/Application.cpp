#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Statistics.h"
#include "Input.h"
#include "PerspectiveCamera.h"
#include "World.h"
#include "Renderer.h"
#include "UserInterface.h"
#include "Player.h"

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
	Input* input = new Input(window);
	PerspectiveCamera* cam = new PerspectiveCamera(input, glm::vec3(0, 100, 0), -80, 0.1f, 1000, 0, 0);
	Renderer renderer(cam, window, world);
	Player player(cam, input);
	Statistics stats;
	UserInterface ui(window, input);

	float prevTime = (float)glfwGetTime(), currentTime = (float)glfwGetTime(), dt = 0;

	while (!glfwWindowShouldClose(window) && !input->GetKeyPressed(Input::KeyVal::ESCAPE)) {
		renderer.BeginFrame();

		input->Update();

		prevTime = currentTime;
		currentTime = (float)glfwGetTime();
		dt = currentTime - prevTime;

		stats.Update(dt);

		renderer.RenderFrame(dt, player.CamUnderWater());

		renderer.RenderPostProcessing(player.CamUnderWater());

		player.Update(dt, world, renderer);

		ui.Update(player);

		world->Update(player.GetPosition(), player.GetChunkCoordinates());

		renderer.EndFrame();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
