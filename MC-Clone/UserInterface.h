#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;

class UserInterface {
public:
	UserInterface(GLFWwindow* window);
	~UserInterface();

	void Draw();
	void DrawCrosshair();
	void DrawDebugInfo(glm::vec3 playerPos, glm::ivec3 playerBlockPos, glm::ivec2 playerChunkPos);

protected:
	GLFWwindow* window;

	bool display;
};