
#include <iostream>

#include <GL/glew.h>
#include <glm/gtx/transform.hpp>

#include "Renderer.h"

Renderer::Renderer(PerspectiveCamera* cam, GLFWwindow* window, World* world) : chunkShader("res/shaders/Chunk.shader"), postProcessShader("res/shaders/UnderWaterPP.shader"), textureAtlas("res/textures/default_texture.png"), sunLight(glm::vec3(0, -1, 0), 1), moonLight(glm::vec3(0, 1, 0), 0), camera(cam), world(world), lineRenderer(cam), window(window) {
	glClearColor(0.41f, 0.64f, 1, 1);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int width;
	int height;
	glfwGetWindowSize(window, &width, &height);

	glActiveTexture(GL_TEXTURE0 + 2);
	glGenTextures(1, &frameTex);
	glBindTexture(GL_TEXTURE_2D, frameTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float fboVertices[8] = {
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f,  1.0f
	};

	glGenBuffers(1, &postProcessVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, postProcessVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), fboVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	chunkShader.Bind();
	textureAtlas.Bind();
	chunkShader.SetUniform1i("u_Texture", 0);
}

Renderer::~Renderer() {
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &frameTex);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteBuffers(1, &postProcessVertexBuffer);
}

void Renderer::BeginFrame() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderFrame(float dt, bool underWater) {

	glm::mat4 projMat = camera->BuildProjectionMatrix();
	glm::mat4 viewMat = camera->BuildViewMatrix();
	glm::mat4 vp = projMat * viewMat;

	skybox.Render(dt, camera);

	glm::vec4 sunRotation = (glm::vec4(sunLight.GetLightDirection(), 1) * glm::rotate(glm::mat4(1), -skybox.GetTimeOfDay() * glm::pi<float>() * 2, glm::vec3(1, 0, 0)));
	glm::vec4 moonRotation = (glm::vec4(moonLight.GetLightDirection(), 1) * glm::rotate(glm::mat4(1), -skybox.GetTimeOfDay() * glm::pi<float>() * 2, glm::vec3(1, 0, 0)));

	chunkShader.Bind();
	chunkShader.SetUniformMat4f("u_VP", vp);
	chunkShader.SetUniform3f("u_CamPos", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
	chunkShader.SetUniform1f("u_Time", glfwGetTime());
	chunkShader.SetUniform1i("u_Water", false);

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
	chunkShader.SetUniform1i("u_UnderWater", underWater);

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

	glDisable(GL_CULL_FACE);

	for (int x = worldExtents.x; x < worldExtents.y; ++x) {
		for (int z = worldExtents.z; z < worldExtents.w; ++z) {
			Chunk* c = world->GetChunkAtPosition(glm::ivec2(x, z));
			if (c) {
				c->BindWater();
				glDrawElements(GL_TRIANGLES, c->GetWaterIndices().size(), GL_UNSIGNED_INT, nullptr);
			}
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_CULL_FACE);

	/*std::vector<glm::ivec2> chunksNearestToFarthest = world->GetChunksNearestToFarthest();
	for (int i = 0; i < chunksNearestToFarthest.size(); ++i) {
		Chunk* c = world->GetChunkAtPosition(chunksNearestToFarthest[i]);
		if (c) {
			c->BindWater();
			glDrawElements(GL_TRIANGLES, c->GetWaterIndices().size(), GL_UNSIGNED_INT, nullptr);
		}
	}*/
}

void Renderer::EndFrame() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Renderer::RenderBlock(BlockAtlas::Type type, glm::ivec3 position) {
	blockRenderer.DrawBlock(type, position);
}

void Renderer::DrawBox(glm::vec3 boxPos, glm::vec3 boxSize, glm::vec4 colour) {

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

void Renderer::RenderPostProcessing(bool underWater) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	postProcessShader.Bind();
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, frameTex);
	postProcessShader.SetUniform1i("u_ScreenTexture", 2);
	postProcessShader.SetUniform1i("u_UnderWater", underWater);
	postProcessShader.SetUniform1f("u_Offset", glfwGetTime());

	glBindBuffer(GL_ARRAY_BUFFER, postProcessVertexBuffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,					// attribute
		2,                  // number of elements per vertex, here (x,y)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
	);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);

	postProcessShader.Unbind();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
