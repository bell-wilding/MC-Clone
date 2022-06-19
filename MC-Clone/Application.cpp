#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "PerspectiveCamera.h"
#include "BlockRenderer.h"
#include "Object.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(void) {

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "MC-Clone", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error initialising GLEW." << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

	glClearColor(0.41f, 0.64f, 1, 1);
    glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::vector<Object> blocks;
	for (int x = -5; x < 5; ++x) {
		for (int z = -5; z < 5; ++z) {
			blocks.push_back(Object(glm::vec3(x, -2, z), glm::vec3(1), glm::vec3(0)));
		}
	}

	BlockRenderer blockRenderer;

	PerspectiveCamera cam(window, glm::vec3(0, 0, 0), 70, 0.1f, 1000, 0, 0);

	float prevTime = 0, currentTime = 0, dt = 0;

	unsigned int frames = 0;
	float fps = 30, elapsedTime = 0;

    while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		blockRenderer.Clear();

		prevTime = currentTime;
		currentTime = (float)glfwGetTime();
		dt = currentTime - prevTime;
		elapsedTime += dt;
		frames++;

		if (elapsedTime > 0.5f) {
			fps = frames * 0.5f + fps * 0.5f;
			frames = 0;
			elapsedTime = 0;
			std::cout << "FPS: " << fps << std::endl;
		}

		cam.Update();

		glm::mat4 projMat = cam.BuildProjectionMatrix();
		glm::mat4 viewMat = cam.BuildViewMatrix();

        //shader.SetUniform4f( "u_Colour", r, 0.4f, 0.8f, 1);

		for (Object block : blocks) {
			blockRenderer.RenderBlock(BlockRenderer::Type::GRASS, block.GetWorldMatrix(), cam);
		}

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}