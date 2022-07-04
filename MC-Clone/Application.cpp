#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "PerspectiveCamera.h"
#include "BlockAtlas.h"
#include "Block.h"
#include "Object.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "Chunk.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "FastNoiseLite.h"
#include <map>
#include <unordered_map>

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
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);

	BlockAtlas blockAtlas;

	FastNoiseLite* noise = new FastNoiseLite();
	noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise->SetFrequency(0.005f);

	int dimensions = 15;
	int startPos = dimensions / 2 - dimensions;
	int endPos = dimensions / 2;

	std::unordered_map<glm::ivec2, Chunk*> world;
	int i = 0;
	for (int x = startPos; x < endPos; ++x) {
		for (int z = startPos; z < endPos; ++z) {
			world[glm::ivec2(x, z)] = new Chunk(glm::vec3(x * 16, 127.5, z * 16), *noise);
			++i;
		}
	}

	i = 0;
	for (int x = startPos; x < endPos; ++x) {
		for (int z = startPos; z < endPos; ++z) {
			world[glm::ivec2(x, z)]->CreateMesh(blockAtlas, world);
			++i;
			std::cout << "Chunks Generated: " << i << std::endl;
		}
	}

	Shader shader("res/shaders/Basic.shader");
	shader.Bind();

	Texture texture("res/textures/default_texture.png");
	texture.Bind();

	shader.SetUniform1i("u_Texture", 0);

	DirectionalLight light(glm::vec3(0.5, 1, 0));
	PerspectiveCamera cam(window, glm::vec3(0, 250, 0), 70, 0.1f, 1000, 0, 0);

	float prevTime = 0, currentTime = 0, dt = 0;

	unsigned int frames = 0;
	float fps = 30, elapsedTime = 0;

	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		glm::ivec2 playerChunkPos((cam.GetPosition().x - 8) / 16, (cam.GetPosition().z - 8) / 16);
		std::cout << playerChunkPos.x << " : " << playerChunkPos.y << std::endl;

		if (!world[playerChunkPos]) {
			world[playerChunkPos] = new Chunk(glm::vec3(playerChunkPos.x * 16, 127.5, playerChunkPos.y * 16), *noise);
			world[playerChunkPos]->CreateMesh(blockAtlas, world);
			
			if (world[glm::vec2(playerChunkPos.x + 1, playerChunkPos.y)])
				world[glm::vec2(playerChunkPos.x + 1, playerChunkPos.y)]->CreateMesh(blockAtlas, world);

			if (world[glm::vec2(playerChunkPos.x - 1, playerChunkPos.y)])
				world[glm::vec2(playerChunkPos.x - 1, playerChunkPos.y)]->CreateMesh(blockAtlas, world);

			if (world[glm::vec2(playerChunkPos.x, playerChunkPos.y + 1)])
				world[glm::vec2(playerChunkPos.x, playerChunkPos.y + 1)]->CreateMesh(blockAtlas, world);

			if (world[glm::vec2(playerChunkPos.x, playerChunkPos.y - 1)])
				world[glm::vec2(playerChunkPos.x, playerChunkPos.y - 1)]->CreateMesh(blockAtlas, world);
		}

		glm::mat4 projMat = cam.BuildProjectionMatrix();
		glm::mat4 viewMat = cam.BuildViewMatrix();

		glm::mat4 vp = projMat * viewMat;

		shader.SetUniformMat4f("u_VP", vp);
		shader.SetUniform3f("u_CamPos", cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z);

		shader.SetUniform3f("u_LightDir", light.GetLightDirection().x, light.GetLightDirection().y, light.GetLightDirection().z);

		for (int x = startPos-10; x < endPos+10; ++x) {
			for (int z = startPos-10; z < endPos+10; ++z) {
				Chunk* c = world[glm::ivec2(x, z)];
				if (c) {
					c->Bind();
					glDrawElements(GL_TRIANGLES, c->GetIndices().size(), GL_UNSIGNED_INT, nullptr);
				}
			}
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}