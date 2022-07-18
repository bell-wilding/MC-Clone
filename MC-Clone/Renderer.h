#pragma once

#include "Skybox.h"
#include "World.h"
#include "PerspectiveCamera.h"
#include "Shader.h"
#include "Texture.h"
#include "LineRenderer.h"
#include "DirectionalLight.h"

class Renderer {
public:
	Renderer(PerspectiveCamera* cam, GLFWwindow* window, World* world);
	~Renderer() {};

	void BeginFrame();
	void RenderFrame(float dt);
	void EndFrame();

	void DrawBox(glm::vec3 boxPos, glm::vec3 boxSize, glm::vec4 colour = glm::vec4(1, 1, 1, 1));

protected:
	Shader chunkShader;
	Texture textureAtlas;

	LineRenderer lineRenderer;

	DirectionalLight sunLight;
	DirectionalLight moonLight;
	Skybox skybox;

	PerspectiveCamera* camera;
	World* world;

	GLFWwindow* window;
};