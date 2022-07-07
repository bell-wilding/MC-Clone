#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "PerspectiveCamera.h"
#include "BlockAtlas.h"
#include "Chunk.h"
#include "Ray.h"

#include "World.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "FastNoiseLite.h"
#include <map>
#include <unordered_map>
#include "Statistics.h"
#include "Player.h"
#include "UserInterface.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

	bool showUI = false;
	bool keyHeld = false;

	World* world = new World(3, 10);
	PerspectiveCamera* cam = new PerspectiveCamera(window, glm::vec3(0, 130, 0), -80, 0.1f, 1000, 0, 0);
	Renderer renderer(cam, window, world);
	Player player(cam);
	Statistics stats;
	UserInterface ui(window);

	bool canBreak = true;
	bool canPlace = true;

	float prevTime = 0, currentTime = 0, dt = 0;

	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		renderer.BeginFrame();

		prevTime = currentTime;
		currentTime = (float)glfwGetTime();
		dt = currentTime - prevTime;

		stats.Update(dt);
		cam->Update(dt);
		player.Update(dt, window, world, renderer);

		renderer.RenderFrame();

		ui.DrawCrosshair();

		if (showUI)
			ui.DrawDebugInfo(cam->GetPosition(), player.GetBlockCoordinates(), player.GetChunkCoordinates());

		if (!keyHeld && glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
			keyHeld = true;
			showUI = !showUI;
			/*cam->SetControlsActive(!showUI);
			if (showUI)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);*/
		}

		if (keyHeld && glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE) {
			keyHeld = false;
		}

		renderer.EndFrame();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}