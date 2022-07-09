#pragma once

#include <glm/glm.hpp>

#include "BlockAtlas.h"
#include "Player.h"

struct GLFWwindow;

class UserInterface {
public:
	UserInterface(GLFWwindow* window);
	~UserInterface();

	void Update(Player& player);

	void Draw();
	BlockAtlas::Type DrawBlockMenu();
	void DrawCrosshair();
	void DrawDebugInfo(glm::vec3 playerPos, glm::ivec3 playerBlockPos, glm::ivec2 playerChunkPos);

protected:
	GLFWwindow* window;

	bool showDebugInfo;
	bool blockMenuActive;
	int pressedKey;
	BlockAtlas::Type selectedBlockType;
};