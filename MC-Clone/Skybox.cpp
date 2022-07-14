#include "Skybox.h"

#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <GL/glew.h>

Skybox::Skybox() : shader("res/shaders/Skybox.shader") {

	float vertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f
	};

	unsigned int indices[] = {
		0, 1, 2, 2, 3, 0
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), &vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices, GL_STATIC_DRAW);

	std::string filenames[6] = {
		"res/textures/skyboxMiddle.png",
		"res/textures/skyboxMiddle.png",
		"res/textures/skyboxBottom.png",
		"res/textures/skyboxTop.png",
		"res/textures/skyboxMiddle.png",
		"res/textures/skyboxMiddle.png"
	};

	struct ImageData {
		unsigned int m_RendererID;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP;
	};

	ImageData images[6];

	glGenTextures(1, &skyboxTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	stbi_set_flip_vertically_on_load(1);

	for (int i = 0; i < 6; ++i) {
		images[i].m_LocalBuffer = stbi_load(filenames[i].c_str(), &images[i].m_Width, &images[i].m_Height, &images[i].m_BPP, 4);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, images[i].m_Width, images[i].m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, images[i].m_LocalBuffer);
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	rotation = 0;
	currentDayTime = 15;
	dayLengthInSeconds = 30;
}

Skybox::~Skybox() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glDeleteTextures(1, &skyboxTex);
}

void Skybox::Render(float dt, PerspectiveCamera* camera) {
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	glm::mat4 projMat = camera->BuildProjectionMatrix();
	glm::mat4 viewMat = camera->BuildViewMatrix();

	shader.Bind();

	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	currentDayTime += dt;
	float timeOfDayFraction = currentDayTime / dayLengthInSeconds;
	rotation = timeOfDayFraction * glm::pi<float>() * 2;
	if (currentDayTime >= dayLengthInSeconds)
		currentDayTime = 0;

	glm::vec4 skyColour(0.545f, 0.741f, 1.0f, 1.0f);
	float t = (timeOfDayFraction > 0.5 ? (timeOfDayFraction - 0.5) * 2 : 1 - timeOfDayFraction * 2) * 0.9;
	fogColour = glm::mix(glm::pow(skyColour, glm::vec4(2.2f)), glm::vec4(0.023, 0.05, 0.101, 1), t);

	shader.SetUniformMat4f("u_ViewMat", viewMat);
	shader.SetUniformMat4f("u_ProjMat", projMat);
	shader.SetUniformMat4f("u_RotationMat", glm::rotate(glm::mat4(1), rotation, glm::vec3(1, 0, 0)));
	shader.SetUniform1f("u_TimeOfDay", currentDayTime / dayLengthInSeconds);
	shader.SetUniform1i("u_SkyTex", 0);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}
