#pragma once

#include "PerspectiveCamera.h"
#include "BlockAtlas.h"
#include "Rigidbody.h"

struct GLFWwindow;
class World;
class Renderer;

class Player {
public:
	Player(PerspectiveCamera* cam);
	~Player() {};

	void Update(float dt, GLFWwindow* window, World* world, Renderer& renderer);

	void ChangeActiveBlockType(BlockAtlas::Type newType) { activeBlockType = newType; }
	BlockAtlas::Type GetActiveBlockType() { return activeBlockType; }

	glm::ivec2 GetChunkCoordinates() { return chunkCoordinates; }
	glm::ivec3 GetBlockCoordinates() { return blockCoordinates; }
	glm::vec3 GetPosition() { return position; }

	PerspectiveCamera* GetPlayerCamera() { return camera; }

	void SetCanBreakBlock(bool canBreak) { canBreakBlock = canBreak; }
	void SetCanPlaceBlock(bool canPlace) { canPlaceBlock = canPlace; }

protected:

	Rigidbody rigidbody;

	glm::vec3 camOffset;

	void HandleBlockInteraction(GLFWwindow* window, World* world, Renderer& renderer);

	void ChangeActiveBlockType(GLFWwindow* window);

	void ApplyPhysics(GLFWwindow* window, float dt);

	BlockAtlas::Block GetNearestBlock(World* world, glm::ivec3& collisionNormal);

	BlockAtlas::Type activeBlockType;

	bool canBreakBlock, canPlaceBlock;

	bool increaseSpeed;
	float movementSpeed;

	PerspectiveCamera* camera;

	glm::vec3 position;
	glm::ivec2 chunkCoordinates;
	glm::ivec3 blockCoordinates;

	int setHZ;
	float setDT;
	float dtOffset;
};