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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "FastNoiseLite.h"
#include <map>

int main(void) {

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1920, 1080, "MC-Clone", NULL, NULL);
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

	unsigned int indices[] = {
			0,  1,  2,  1,  3,  2,
			4,  5,  6,  5,  7,  6,
			8,  9,  10, 9, 11, 10,
			12, 13, 14, 13, 15, 14,
			18, 17, 16, 18, 19, 17,
			20, 21, 22, 21, 23, 22
	};

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int blockBuffer;
	glGenBuffers(1, &blockBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, blockBuffer);
	glBufferData(GL_ARRAY_BUFFER, 8 * 4 * 6 * sizeof(float), &blockAtlas.GetBlockVertexArray(BlockAtlas::Type::GRASS)[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(5 * sizeof(float)));

	unsigned int indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	Shader shader("res/shaders/Basic.shader");
	shader.Bind();

	Texture texture("res/textures/default_texture.png");
	texture.Bind();

	shader.SetUniform1i("u_Texture", 0);

	FastNoiseLite* noise = new FastNoiseLite(3);
	noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise->SetFrequency(0.005f);

	Chunk* chunks[900];

	int i = 0;
	for (int x = 0; x < 30; ++x) {
		for (int z = 0; z < 30; ++z) {
			chunks[i] = new Chunk(glm::vec3(7.5 + x * 16, 127.5, 7.5 + z * 16), *noise);
			++i;
		}
	}

	std::vector<glm::vec4> grassBlocks;
	std::vector<glm::vec4> dirtBlocks;
	std::vector<glm::vec4> stoneBlocks;

	for (Chunk* chunk : chunks)
	{
		std::vector<BlockAtlas::Block> blocks = chunk->GetBlocks();

		for (BlockAtlas::Block block : blocks) {
			if (block.type == BlockAtlas::Type::GRASS) {
				grassBlocks.push_back(glm::vec4(block.position, 1));
			}
			else if (block.type == BlockAtlas::Type::DIRT) {
				dirtBlocks.push_back(glm::vec4(block.position, 1));
			}
			else if (block.type == BlockAtlas::Type::STONE) {
				stoneBlocks.push_back(glm::vec4(block.position, 1));
			}
		}
	}

	unsigned int worldPosBuffer;
	glGenBuffers(1, &worldPosBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, worldPosBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, grassBlocks.size() * sizeof(glm::vec4), &grassBlocks[0], GL_DYNAMIC_COPY);

	DirectionalLight light(glm::vec3(0, -1, 0));
	PerspectiveCamera cam(window, glm::vec3(0, 200, 0), 70, 0.1f, 1000, 0, 0);

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

		glm::mat4 projMat = cam.BuildProjectionMatrix();
		glm::mat4 viewMat = cam.BuildViewMatrix();

		glm::mat4 vp = projMat * viewMat;

		shader.SetUniformMat4f("u_VP", vp);
		shader.SetUniform3f("u_CamPos", cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z);

		light.RotateDirection(glm::vec3(0, 0, 1), 0.01f);
		shader.SetUniform3f("u_LightDir", light.GetLightDirection().x, light.GetLightDirection().y, light.GetLightDirection().z);

		//glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, nullptr);

		glBufferData(GL_ARRAY_BUFFER, 8 * 4 * 6 * sizeof(float), &blockAtlas.GetBlockVertexArray(BlockAtlas::Type::GRASS)[0], GL_STATIC_DRAW);
		glBufferData(GL_SHADER_STORAGE_BUFFER, grassBlocks.size() * sizeof(glm::vec4), &grassBlocks[0], GL_DYNAMIC_COPY);
		glDrawElementsInstanced(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0, grassBlocks.size());

		glBufferData(GL_ARRAY_BUFFER, 8 * 4 * 6 * sizeof(float), &blockAtlas.GetBlockVertexArray(BlockAtlas::Type::DIRT)[0], GL_STATIC_DRAW);
		glBufferData(GL_SHADER_STORAGE_BUFFER, dirtBlocks.size() * sizeof(glm::vec4), &dirtBlocks[0], GL_DYNAMIC_COPY);
		glDrawElementsInstanced(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0, dirtBlocks.size());

		glBufferData(GL_ARRAY_BUFFER, 8 * 4 * 6 * sizeof(float), &blockAtlas.GetBlockVertexArray(BlockAtlas::Type::STONE)[0], GL_STATIC_DRAW);
		glBufferData(GL_SHADER_STORAGE_BUFFER, stoneBlocks.size() * sizeof(glm::vec4), &stoneBlocks[0], GL_DYNAMIC_COPY);
		glDrawElementsInstanced(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0, stoneBlocks.size());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}