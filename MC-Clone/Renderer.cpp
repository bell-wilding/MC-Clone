#include "Renderer.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

Renderer::Renderer(PerspectiveCamera* cam, GLFWwindow* window, World* world) : chunkShader("res/shaders/Chunk.shader"), textureAtlas("res/textures/default_texture.png"), sunLight(glm::vec3(0, -1, 0), 1), moonLight(glm::vec3(0, 1, 0), 0), camera(cam), world(world), lineRenderer(cam), window(window) {
	glClearColor(0.41f, 0.64f, 1, 1);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	chunkShader.Bind();
	textureAtlas.Bind();
	chunkShader.SetUniform1i("u_Texture", 0);
}

void Renderer::BeginFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderFrame(float dt) {
	glm::mat4 projMat = camera->BuildProjectionMatrix();
	glm::mat4 viewMat = camera->BuildViewMatrix();
	glm::mat4 vp = projMat * viewMat;

	skybox.Render(dt, camera);

	glm::vec4 sunRotation = (glm::vec4(sunLight.GetLightDirection(), 1) * glm::rotate(glm::mat4(1), -skybox.GetTimeOfDay() * glm::pi<float>() * 2, glm::vec3(1, 0, 0)));
	glm::vec4 moonRotation = (glm::vec4(moonLight.GetLightDirection(), 1) * glm::rotate(glm::mat4(1), -skybox.GetTimeOfDay() * glm::pi<float>() * 2, glm::vec3(1, 0, 0)));

	chunkShader.Bind();
	chunkShader.SetUniformMat4f("u_VP", vp);
	chunkShader.SetUniform3f("u_CamPos", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);

	float intensity = 0;
	if (skybox.GetTimeOfDay() > 0.75) {
		intensity = glm::clamp(1 - ((skybox.GetTimeOfDay() - 0.75) * 8.0), 0.0, 1.0);
	}
	else if (skybox.GetTimeOfDay() > 0.2) {
		intensity = glm::clamp((skybox.GetTimeOfDay() - 0.2) * 8.0, 0.0, 1.0);
	}
	sunLight.SetIntensity(intensity);
	moonLight.SetIntensity((1 - intensity) * 0.3f);

	chunkShader.SetUniform4f("u_SunLight", sunRotation.x, sunRotation.y, sunRotation.z, sunLight.GetLightIntensity());
	chunkShader.SetUniform4f("u_MoonLight", moonRotation.x, moonRotation.y, moonRotation.z, moonLight.GetLightIntensity());
	chunkShader.SetUniform4f("u_FogColour", skybox.GetFogColour().r, skybox.GetFogColour().g, skybox.GetFogColour().b, skybox.GetFogColour().a);

	glm::ivec4 worldExtents = world->GetWorldExtents();
	for (int x = worldExtents.x; x < worldExtents.y; ++x) {
		for (int z = worldExtents.z; z < worldExtents.w; ++z) {
			Chunk* c = world->GetChunkAtPosition(glm::ivec2(x, z));
			if (c) {
				c->Bind();
				glDrawElements(GL_TRIANGLES, c->GetIndices().size(), GL_UNSIGNED_INT, nullptr);
			}
		}
	}
}

void Renderer::EndFrame() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Renderer::DrawBox(glm::vec3 boxPos, glm::vec3 boxSize) {

	glm::vec3 vertices[8] = {
		boxPos + boxSize * glm::vec3(1, 1, 1),
		boxPos + boxSize * glm::vec3(1, -1, 1),
		boxPos + boxSize * glm::vec3(1, -1, -1),
		boxPos + boxSize * glm::vec3(1, 1, -1),
		boxPos + boxSize * glm::vec3(-1, 1, 1),
		boxPos + boxSize * glm::vec3(-1, -1, 1),
		boxPos + boxSize * glm::vec3(-1, -1, -1),
		boxPos + boxSize * glm::vec3(-1, 1, -1)
	};

	lineRenderer.DrawLine(vertices[0], vertices[1]);
	lineRenderer.DrawLine(vertices[1], vertices[2]);
	lineRenderer.DrawLine(vertices[2], vertices[3]);
	lineRenderer.DrawLine(vertices[3], vertices[0]);
	lineRenderer.DrawLine(vertices[0], vertices[4]);
	lineRenderer.DrawLine(vertices[4], vertices[7]);
	lineRenderer.DrawLine(vertices[7], vertices[3]);
	lineRenderer.DrawLine(vertices[7], vertices[3]);
	lineRenderer.DrawLine(vertices[7], vertices[6]);
	lineRenderer.DrawLine(vertices[6], vertices[2]);
	lineRenderer.DrawLine(vertices[1], vertices[5]);
	lineRenderer.DrawLine(vertices[4], vertices[5]);
	lineRenderer.DrawLine(vertices[6], vertices[5]);
}
