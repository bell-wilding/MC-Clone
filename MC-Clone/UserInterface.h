#pragma once

#include <glm/glm.hpp>

#include "BlockAtlas.h"
#include "Player.h"

struct GLFWwindow;

class UserInterface {
public:
	UserInterface(GLFWwindow* window, Input* input);
	~UserInterface();

	void Update(Player& player);

	void Draw();
	BlockAtlas::Type DrawBlockMenu();
	void DrawCrosshair();
	void DrawDebugInfo(glm::vec3 playerPos, glm::ivec3 playerBlockPos, glm::ivec2 playerChunkPos);

protected:
	Input* input;

	bool showDebugInfo;
	bool blockMenuActive;
	BlockAtlas::Type selectedBlockType;
};