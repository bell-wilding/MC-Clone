#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Renderer::Renderer(PerspectiveCamera* cam, GLFWwindow* window, World* world) : chunkShader("res/shaders/Chunk.shader"), textureAtlas("res/textures/default_texture.png"), light(glm::vec3(0.5, 1, 0)), camera(cam), world(world), lineRenderer(cam), window(window) {
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

	//light.RotateDirection(glm::vec3(1, 0, 0), skybox.GetTimeOfDay() * 360);

	chunkShader.Bind();
	chunkShader.SetUniformMat4f("u_VP", vp);
	chunkShader.SetUniform3f("u_CamPos", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);

	chunkShader.SetUniform3f("u_LightDir", light.GetLightDirection().x, light.GetLightDirection().y, light.GetLightDirection().z);
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
