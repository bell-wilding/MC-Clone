#pragma once

#include "Skybox.h"
#include "World.h"
#include "PerspectiveCamera.h"
#include "Shader.h"
#include "Texture.h"
#include "LineRenderer.h"
#include "DirectionalLight.h"
#include "BlockRenderer.h"

class Renderer {
public:
	Renderer(PerspectiveCamera* cam, GLFWwindow* window, World* world);
	~Renderer();

	void BeginFrame();
	void RenderFrame(float dt, bool underWater);
	void EndFrame();

	void RenderBlock(BlockAtlas::Type type, glm::ivec3 position);

	void DrawBox(glm::vec3 boxPos, glm::vec3 boxSize, glm::vec4 colour = glm::vec4(1, 1, 1, 1));

	void RenderPostProcessing(bool underWater);

protected:

	Shader chunkShader;
	Texture textureAtlas;

	LineRenderer lineRenderer;
	BlockRenderer blockRenderer;

	DirectionalLight sunLight;
	DirectionalLight moonLight;
	Skybox skybox;

	PerspectiveCamera* camera;
	World* world;

	GLFWwindow* window;

	unsigned int postProcessVertexBuffer;

	Shader postProcessShader;
	unsigned int fbo;
	unsigned int rbo;
	unsigned int frameTex;
};