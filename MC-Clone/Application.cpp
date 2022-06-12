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

    glfwSwapInterval(1);

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
	  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	  -0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	  -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
	  // Face 2
	  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	   0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	  -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
	   0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
	   // Face 3
	   0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	   0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
	   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	   // Face 4
	   0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	  -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	   0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	  -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	   // Top
	  -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
	   0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
	   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	   // Bottom
	 -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	 -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	  0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
	  0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
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

	PerspectiveCamera cam(window, glm::vec3(0, 0, 0), 70, 0.1f, 1000, 0, 0);

	std::vector<Object> objects;
	for (int x = -5; x < 5; ++x) {
		for (int z = -5; z < 5; ++z) {
			objects.push_back(Object(glm::vec3(x, -2, z), glm::vec3(1, 1, 1), glm::vec3(180, 0, 0)));
		}
	}

	Shader shader("res/shaders/Basic.shader");
	shader.Bind();

    Texture texture1("res/textures/grass.jpg");
	texture1.Bind();

    va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();

	Renderer renderer;

    float r = 0.0f;
    float increment = 0.01f;

    while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		renderer.Clear();

		cam.Update();

		glm::mat4 projMat = cam.BuildProjectionMatrix();
		glm::mat4 viewMat = cam.BuildViewMatrix();

		shader.Bind();
        texture1.Bind();
        shader.SetUniform1i("u_Texture", 0);
        //shader.SetUniform4f( "u_Colour", r, 0.4f, 0.8f, 1);

		for (auto& object : objects) {
			glm::mat4 mvp = projMat * viewMat * object.GetWorldMatrix();
			shader.SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(va, ib, shader);
		}

        if (r > 1.0f)
            increment = -0.01f;
        else if (r < 0.0f)
            increment = 0.01f;

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}