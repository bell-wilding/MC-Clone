#pragma once

#include "Input.h"
#include "PerspectiveCamera.h"
#include "BlockAtlas.h"
#include "Rigidbody.h"
#include "AABB.h"
#include "CollisionDetection.h"

struct GLFWwindow;
class World;
class Renderer;

class Player {
public:
	Player(PerspectiveCamera* cam, Input* input);
	~Player() {};

	void Update(float dt, World* world, Renderer& renderer);

	void ChangeActiveBlockType(BlockAtlas::Type newType) { activeBlockType = newType; }
	BlockAtlas::Type GetActiveBlockType() { return activeBlockType; }

	glm::ivec2 GetChunkCoordinates() { return chunkCoordinates; }
	glm::ivec3 GetBlockCoordinates() { return blockCoordinates; }
	glm::vec3 GetPosition() { return rigidbody.GetPosition(); }

	PerspectiveCamera* GetPlayerCamera() { return camera; }

	void SetCanBreakBlock(bool canBreak) { canBreakBlock = canBreak; }
	void SetCanPlaceBlock(bool canPlace) { canPlaceBlock = canPlace; }

	void ToggleFlyingCamMode(bool flyingCam);

protected:

	Rigidbody rigidbody;

	void UpdateKeys();

	void HandleBlockInteraction(World* world, Renderer& renderer);

	void ChangeActiveBlockType();

	void ApplyPhysics(World* world, float dt);

	void CollisionDetection(World* world);

	void ImpulseCollisionResolution(CollisionDetection::ContactPoint& point);

	void Jump();

	BlockAtlas::Block GetNearestBlock(World* world, glm::ivec3& collisionNormal);

	BlockAtlas::Type activeBlockType;

	bool grounded;
	float jumpVelocity;
	float jumpCooldown;
	float jumpCooldownTimer;

	bool increaseSpeed;
	bool decreaseSpeed;
	float movementSpeed;

	float walkSpeed;
	float flySpeed;

	PerspectiveCamera* camera;
	glm::vec3 camOffset;

	glm::ivec2 chunkCoordinates;
	glm::ivec3 blockCoordinates;

	int setHZ;
	float setDT;
	float dtOffset;

	bool collisionsEnabled;

	AABB boxCollider;

	Input* input;

	bool flyingCamMode;
	bool inWater;
	float waterJumpForce;

	float breakBlockDuration;
	float breakBlockTimer;
	bool instantBreak;

	bool canBreakBlock, canPlaceBlock, breakingBlock;
	glm::ivec3 breakingBlockPosition;
};