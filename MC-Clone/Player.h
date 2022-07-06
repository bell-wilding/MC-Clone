#pragma once

#include "PerspectiveCamera.h"
#include "BlockAtlas.h"

struct GLFWwindow;
class World;
class Renderer;

class Player {
public:
	Player(PerspectiveCamera* cam);
	~Player() {};

	void Update(float dt, GLFWwindow* window, World* world, Renderer& renderer);

protected:

	void HandleBlockInteraction(GLFWwindow* window, World* world, Renderer& renderer);

	void ChangeActiveBlockType(GLFWwindow* window);

	BlockAtlas::Block GetNearestBlock(World* world, glm::ivec3& collisionNormal);

	BlockAtlas::Type activeBlockType;

	bool canBreakBlock, canPlaceBlock;

	PerspectiveCamera* camera;
	glm::ivec2 chunkCoordinates;
	glm::ivec3 blockCoordinates;
};