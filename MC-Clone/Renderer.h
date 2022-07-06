#pragma once

#include "Shader.h"
#include "Texture.h"
#include "LineRenderer.h"
#include "DirectionalLight.h"
#include "World.h"
#include "PerspectiveCamera.h"

class Renderer {
public:
	Renderer(PerspectiveCamera* cam, GLFWwindow* window, World* world);
	~Renderer() {};

	void BeginFrame();
	void RenderFrame();
	void EndFrame();

	void DrawBox(glm::vec3 boxPos, glm::vec3 boxSize);

protected:
	Shader chunkShader;
	Texture textureAtlas;

	LineRenderer lineRenderer;

	DirectionalLight light;

	PerspectiveCamera* camera;
	World* world;

	GLFWwindow* window;
};