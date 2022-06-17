#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "PerspectiveCamera.h"
#include "Block.h"

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

	float vertices[] = {
		// Face 1
	  -0.5f, -0.5f, -0.5f, 3.0f, 15.0f,
	  -0.5f, -0.5f,  0.5f, 4.0f, 15.0f,
	  -0.5f,  0.5f, -0.5f, 3.0f, 16.0f,
	  -0.5f,  0.5f,  0.5f, 4.0f, 16.0f,
	  // Face 2
	  -0.5f, -0.5f,  0.5f, 3.0f, 15.0f,
	   0.5f, -0.5f,  0.5f, 4.0f, 15.0f,
	  -0.5f,  0.5f,  0.5f, 3.0f, 16.0f,
	   0.5f,  0.5f,  0.5f, 4.0f, 16.0f,
	   // Face 3
	   0.5f, -0.5f,  0.5f, 3.0f, 15.0f,
	   0.5f, -0.5f, -0.5f, 4.0f, 15.0f,
	   0.5f,  0.5f,  0.5f, 3.0f, 16.0f,
	   0.5f,  0.5f, -0.5f, 4.0f, 16.0f,
	   // Face 4
	   0.5f, -0.5f, -0.5f, 3.0f, 15.0f,
	  -0.5f, -0.5f, -0.5f, 4.0f, 15.0f,
	   0.5f,  0.5f, -0.5f, 3.0f, 16.0f,
	  -0.5f,  0.5f, -0.5f, 4.0f, 16.0f,
	   // Top
	  -0.5f,  0.5f,  0.5f, 0.0f, 15.0f,
	  -0.5f,  0.5f, -0.5f, 1.0f, 15.0f,
	   0.5f,  0.5f,  0.5f, 0.0f, 16.0f,
	   0.5f,  0.5f, -0.5f, 1.0f, 16.0f,
	   // Bottom
	 -0.5f, -0.5f,  0.5f, 2.0f, 15.0f,
	 -0.5f, -0.5f, -0.5f, 3.0f, 15.0f,
	  0.5f, -0.5f,  0.5f, 2.0f, 16.0f,
	  0.5f, -0.5f, -0.5f, 3.0f, 16.0f,
	}; 

	unsigned int indices[] = {
		0, 1, 2, 2, 3, 1,
		4, 5, 6, 6, 7, 5,
		8, 9, 10, 10, 11, 9,
		12, 13, 14, 14, 15, 13,
		16, 17, 18, 18, 19, 17,
		20, 21, 22, 22, 23, 21
	}; 

    VertexArray va;
    VertexBuffer vb(vertices, 6 * 4 * 5 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 6 * 6);

	float vertices2[] = {
		// Face 1
	  -0.5f, -0.5f, -0.5f, 4.0f, 14.0f,
	  -0.5f, -0.5f,  0.5f, 5.0f, 14.0f,
	  -0.5f,  0.5f, -0.5f, 4.0f, 15.0f,
	  -0.5f,  0.5f,  0.5f, 5.0f, 15.0f,
	  // Face 2
	  -0.5f, -0.5f,  0.5f, 4.0f, 14.0f,
	   0.5f, -0.5f,  0.5f, 5.0f, 14.0f,
	  -0.5f,  0.5f,  0.5f, 4.0f, 15.0f,
	   0.5f,  0.5f,  0.5f, 5.0f, 15.0f,
	   // Face 3
	   0.5f, -0.5f,  0.5f, 4.0f, 14.0f,
	   0.5f, -0.5f, -0.5f, 5.0f, 14.0f,
	   0.5f,  0.5f,  0.5f, 4.0f, 15.0f,
	   0.5f,  0.5f, -0.5f, 5.0f, 15.0f,
	   // Face 4
	   0.5f, -0.5f, -0.5f, 4.0f, 14.0f,
	  -0.5f, -0.5f, -0.5f, 5.0f, 14.0f,
	   0.5f,  0.5f, -0.5f, 4.0f, 15.0f,
	  -0.5f,  0.5f, -0.5f, 5.0f, 15.0f,
	  // Top
	 -0.5f,  0.5f,  0.5f, 5.0f, 14.0f,
	 -0.5f,  0.5f, -0.5f, 6.0f, 14.0f,
	  0.5f,  0.5f,  0.5f, 5.0f, 15.0f,
	  0.5f,  0.5f, -0.5f, 6.0f, 15.0f,
	  // Bottom
	-0.5f, -0.5f,  0.5f, 5.0f, 14.0f,
	-0.5f, -0.5f, -0.5f, 6.0f, 14.0f,
	 0.5f, -0.5f,  0.5f, 5.0f, 15.0f,
	 0.5f, -0.5f, -0.5f, 6.0f, 15.0f,
	};

	unsigned int indices2[] = {
		0, 1, 2, 2, 3, 1,
		4, 5, 6, 6, 7, 5,
		8, 9, 10, 10, 11, 9,
		12, 13, 14, 14, 15, 13,
		16, 17, 18, 18, 19, 17,
		20, 21, 22, 22, 23, 21
	};

	VertexArray va2;
	VertexBuffer vb2(vertices2, 6 * 4 * 5 * sizeof(float));
	VertexBufferLayout layout2;
	layout2.Push<float>(3);
	layout2.Push<float>(2);
	va2.AddBuffer(vb2, layout);

	IndexBuffer ib2(indices2, 6 * 6);

	float vertices3[] = {
		// Face 1
	  -0.5f, -0.5f, -0.5f, 4.0f, 12.0f,
	  -0.5f, -0.5f,  0.5f, 5.0f, 12.0f,
	  -0.5f,  0.5f, -0.5f, 4.0f, 13.0f,
	  -0.5f,  0.5f,  0.5f, 5.0f, 13.0f,
	  // Face 2
	  -0.5f, -0.5f,  0.5f, 4.0f, 12.0f,
	   0.5f, -0.5f,  0.5f, 5.0f, 12.0f,
	  -0.5f,  0.5f,  0.5f, 4.0f, 13.0f,
	   0.5f,  0.5f,  0.5f, 5.0f, 13.0f,
	   // Face 3
	   0.5f, -0.5f,  0.5f, 4.0f, 12.0f,
	   0.5f, -0.5f, -0.5f, 5.0f, 12.0f,
	   0.5f,  0.5f,  0.5f, 4.0f, 13.0f,
	   0.5f,  0.5f, -0.5f, 5.0f, 13.0f,
	   // Face 4
	   0.5f, -0.5f, -0.5f, 4.0f, 12.0f,
	  -0.5f, -0.5f, -0.5f, 5.0f, 12.0f,
	   0.5f,  0.5f, -0.5f, 4.0f, 13.0f,
	  -0.5f,  0.5f, -0.5f, 5.0f, 13.0f,
	  // Top
	 -0.5f,  0.5f,  0.5f, 4.0f, 12.0f,
	 -0.5f,  0.5f, -0.5f, 5.0f, 12.0f,
	  0.5f,  0.5f,  0.5f, 4.0f, 13.0f,
	  0.5f,  0.5f, -0.5f, 5.0f, 13.0f,
	  // Bottom
	-0.5f, -0.5f,  0.5f, 4.0f, 12.0f,
	-0.5f, -0.5f, -0.5f, 5.0f, 12.0f,
	 0.5f, -0.5f,  0.5f, 4.0f, 13.0f,
	 0.5f, -0.5f, -0.5f, 5.0f, 13.0f,
	};

	unsigned int indices3[] = {
		0, 1, 2, 2, 3, 1,
		4, 5, 6, 6, 7, 5,
		8, 9, 10, 10, 11, 9,
		12, 13, 14, 14, 15, 13,
		16, 17, 18, 18, 19, 17,
		20, 21, 22, 22, 23, 21
	};

	VertexArray va3;
	VertexBuffer vb3(vertices3, 6 * 4 * 5 * sizeof(float));
	VertexBufferLayout layout3;
	layout3.Push<float>(3);
	layout3.Push<float>(2);
	va3.AddBuffer(vb3, layout);

	IndexBuffer ib3(indices3, 6 * 6);

	/*Block grass(Block::Type::GRASS, glm::vec3(0), glm::vec2(1, 16), glm::vec2(3, 16), glm::vec2(4, 16));
	grass.SetPosition(glm::vec3(0, -2, 0));*/

	/*std::vector<Block> blocks;
	for (int x = -10; x < 10; ++x) {
		for (int z = -10; z < 10; ++z) {
			grass.SetPosition(glm::vec3(x, -2, z));
			blocks.push_back(Block(grass));
		}
	}*/

	PerspectiveCamera cam(window, glm::vec3(0, 0, 0), 70, 0.1f, 1000, 0, 0);

	std::vector<Object> grassBlocks;
	for (int x = -10; x < 10; ++x) {
		for (int z = -10; z < 10; ++z) {
			grassBlocks.push_back(Object(glm::vec3(x, -2, z), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
		}
	}

	std::vector<Object> treeBlocks;
	for (int y = -1; y < 3; ++y) {
		treeBlocks.push_back(Object(glm::vec3(0, y, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	}

	std::vector<Object> leafBlocks;
	leafBlocks.push_back(Object(glm::vec3(1, 2, 1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(-1, 2, 1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(1, 2, -1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(-1, 2, -1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(0, 2, 1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(-1, 2, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(0, 2, -1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(1, 2, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));

	leafBlocks.push_back(Object(glm::vec3(1, 3, 1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(-1, 3, 1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(1, 3, -1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(-1, 3, -1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(0, 3, 1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(-1, 3, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(0, 3, -1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(1, 3, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));

	leafBlocks.push_back(Object(glm::vec3(0, 4, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(0, 4, 1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(-1, 4, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(0, 4, -1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	leafBlocks.push_back(Object(glm::vec3(1, 4, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));


	Object tree(glm::vec3(0, -1, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));

	Shader shader("res/shaders/Basic.shader");
	shader.Bind();

    Texture texture1("res/textures/default_texture.png");
	texture1.Bind();

    va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();

	Renderer renderer;

	float prevTime = 0, currentTime = 0, dt = 0;

	unsigned int frames = 0;
	float fps = 30, elapsedTime = 0;

    while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		renderer.Clear();

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

		shader.Bind();
        texture1.Bind();
        shader.SetUniform1i("u_Texture", 0);
        //shader.SetUniform4f( "u_Colour", r, 0.4f, 0.8f, 1);

		for (auto& object : grassBlocks) {
			glm::mat4 mvp = projMat * viewMat * object.GetWorldMatrix();
			shader.SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(va, ib, shader);
		}

		for (auto& object : treeBlocks) {
			glm::mat4 mvp = projMat * viewMat * object.GetWorldMatrix();
			shader.SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(va2, ib2, shader);
		}

		for (auto& object : leafBlocks) {
			glm::mat4 mvp = projMat * viewMat * object.GetWorldMatrix();
			shader.SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(va3, ib3, shader);
		}

		/*glm::mat4 mvp = projMat * viewMat * grass.GetWorldMatrix();
		shader.SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(va, ib, shader);*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}