#include <GLFW/glfw3.h>

#include "Renderer.h"

#include "Player.h"

#include "Renderer.h"
#include "Print.h"

#include "Ray.h"
#include "World.h"
#include <iostream>

#include <algorithm>

Player::Player(PerspectiveCamera* cam, Input* input) : camera(cam), input(input), camOffset(glm::vec3(0, 1, 0)), rigidbody(cam->GetPosition() - camOffset) {
	chunkCoordinates = glm::ivec2((camera->GetPosition().x - 8) / 16, (camera->GetPosition().z - 8) / 16);
	blockCoordinates = glm::ivec3(std::round(camera->GetPosition().x), std::round(camera->GetPosition().y), std::round(camera->GetPosition().z));

	activeBlockType = BlockAtlas::Type::GRASS;

	boxCollider = AABB(glm::vec3(0.25f, 0.9f, 0.25f), glm::vec3(0, 0.35f, 0));

	walkSpeed = 60;
	flySpeed = 500;
	jumpVelocity = 12.0f;
	jumpCooldown = 0.4f;
	jumpCooldownTimer = 0.0f;
	movementSpeed = walkSpeed;

	breakBlockDuration = 1;
	breakBlockTimer = 0;
	instantBreak = false;

	canBreakBlock = true;
	canPlaceBlock = true;

	increaseSpeed = false;

	setHZ = 60;
	setDT = 1.0f / setHZ;
	dtOffset = 0;

	flyingCamMode = false;
	collisionsEnabled = true;
}

void Player::Update(float dt, World* world, Renderer& renderer) {
	chunkCoordinates = glm::ivec2(camera->GetPosition().x / 16, camera->GetPosition().z / 16);
	blockCoordinates = glm::ivec3(std::round(camera->GetPosition().x), std::round(camera->GetPosition().y), std::round(camera->GetPosition().z));

	jumpCooldownTimer += dt;
	if (breakingBlock)
		breakBlockTimer += dt;

	ApplyPhysics(world, dt);
	camera->Update(dt);

	UpdateKeys();

	ChangeActiveBlockType();
	HandleBlockInteraction(world, renderer);
}

void Player::ToggleFlyingCamMode(bool flyingCam) {
	flyingCamMode = flyingCam;
	rigidbody.SetApplyGravity(!flyingCam);
	movementSpeed = flyingCam ? flySpeed : walkSpeed;
	collisionsEnabled = !flyingCam;
	rigidbody.SetDamping(glm::vec3(8.0f, flyingCam ? 8.0f : 1.0f, 8.0f));
}

void Player::UpdateKeys() {
	if (input->GetKeyPressed(Input::KeyVal::X)) {
		ToggleFlyingCamMode(!flyingCamMode);
	}

	if (input->GetKeyPressed(Input::KeyVal::B)) {
		instantBreak = !instantBreak;
	}

	if (flyingCamMode && !increaseSpeed && input->GetKeyDown(Input::KeyVal::SPACE)) {
		increaseSpeed = true;
		movementSpeed *= 4;
	}

	if (flyingCamMode && increaseSpeed && !input->GetKeyDown(Input::KeyVal::SPACE)) {
		increaseSpeed = false;
		movementSpeed /= 4;
	}


	if (!flyingCamMode && !decreaseSpeed && input->GetKeyDown(Input::KeyVal::LEFT_SHIFT)) {
		decreaseSpeed = true;
		movementSpeed /= 4;
		camOffset -= glm::vec3(0, 0.4f, 0);
	}

	if (!flyingCamMode && decreaseSpeed && !input->GetKeyDown(Input::KeyVal::LEFT_SHIFT)) {
		decreaseSpeed = false;
		movementSpeed *= 4;
		camOffset += glm::vec3(0, 0.4f, 0);
	}

	if (input->GetKeyDown(Input::KeyVal::W)) {
		rigidbody.AddForce(camera->ForwardVector() * glm::vec3(1, (flyingCamMode ? 1 : 0), 1) * movementSpeed);
	}
	if (input->GetKeyDown(Input::KeyVal::S)) {
		rigidbody.AddForce(camera->ForwardVector() * glm::vec3(1, (flyingCamMode ? 1 : 0), 1) * -movementSpeed);
	}
	if (input->GetKeyDown(Input::KeyVal::D)) {
		rigidbody.AddForce(camera->RightVector() * movementSpeed);
	}
	if (input->GetKeyDown(Input::KeyVal::A)) {
		rigidbody.AddForce(camera->RightVector() * -movementSpeed);
	}
	if (flyingCamMode && input->GetKeyDown(Input::KeyVal::LEFT_SHIFT)) {
		rigidbody.AddForce(glm::vec3(0, 1, 0) * movementSpeed);
	}
	if (flyingCamMode && input->GetKeyDown(Input::KeyVal::LEFT_CONTROL)) {
		rigidbody.AddForce(glm::vec3(0, -1, 0) * movementSpeed);
	}

}

void Player::HandleBlockInteraction(World* world, Renderer& renderer) {
	glm::ivec3 collisionNormal;
	BlockAtlas::Block block = GetNearestBlock(world, collisionNormal);

	if (block.type != BlockAtlas::Type::AIR && block.type != BlockAtlas::Type::WATER && block.type != BlockAtlas::Type::WATER_TOP) {
		if (input->GetMouseButtonDown(Input::MouseButton::LEFT)) {
			if ((instantBreak && canBreakBlock) || block.isFauna) {
				breakingBlock = false;
				breakBlockTimer = 0;
				world->DestroyBlock(block.position);
				canBreakBlock = false;
			}
			else {
				breakingBlock = true;
				if (breakingBlockPosition != glm::ivec3(block.position)) {
					breakBlockTimer = 0;
				}
				breakingBlockPosition = block.position;
			}
		}

		if (breakingBlock) {
			int breakTexture = (int)BlockAtlas::Type::BREAK_1 + (int)((breakBlockTimer / breakBlockDuration * 10));
			renderer.RenderBlock((BlockAtlas::Type)breakTexture, block.position);
		}
		if (breakingBlock && breakBlockTimer >= breakBlockDuration) {
			breakingBlock = false;
			breakBlockTimer = 0;
			world->DestroyBlock(block.position);
			canBreakBlock = false;
		}
		if (canPlaceBlock && input->GetMouseButtonDown(Input::MouseButton::RIGHT)) {
			world->PlaceBlock((glm::ivec3)block.position + collisionNormal, activeBlockType);
			canPlaceBlock = false;
		}
		renderer.DrawBox(block.position, glm::vec3(0.502, 0.502, 0.502));
	}

	if (!input->GetMouseButtonDown(Input::MouseButton::LEFT)) {
		breakBlockTimer = 0;
		breakingBlock = false;
		canBreakBlock = true;
	}

	if (!input->GetMouseButtonDown(Input::MouseButton::RIGHT)) {
		canPlaceBlock = true;
	}
}

void Player::ChangeActiveBlockType() {
	if (input->GetKeyPressed(Input::KeyVal::ONE)) {
		activeBlockType = BlockAtlas::GRASS;
	}
	if (input->GetKeyPressed(Input::KeyVal::TWO)) {
		activeBlockType = BlockAtlas::DIRT;
	}
	if (input->GetKeyPressed(Input::KeyVal::THREE)) {
		activeBlockType = BlockAtlas::STONE;
	}
	if (input->GetKeyPressed(Input::KeyVal::FOUR)) {
		activeBlockType = BlockAtlas::OAK_LOG;
	}
	if (input->GetKeyPressed(Input::KeyVal::FIVE)) {
		activeBlockType = BlockAtlas::LEAF;
	}
	if (input->GetKeyPressed(Input::KeyVal::SIX)) {
		activeBlockType = BlockAtlas::WOOD_PLANK;
	}
	if (input->GetKeyPressed(Input::KeyVal::SEVEN)) {
		activeBlockType = BlockAtlas::WINDOW;
	}
	if (input->GetKeyPressed(Input::KeyVal::EIGHT)) {
		activeBlockType = BlockAtlas::COBBLESTONE;
	}
	if (input->GetKeyPressed(Input::KeyVal::NINE)) {
		activeBlockType = BlockAtlas::SAND;
	}
}

void Player::ApplyPhysics(World* world, float dt) {

	dtOffset += dt;
	
	while (dtOffset >= setDT) {

		if (collisionsEnabled)
			CollisionDetection(world);

		rigidbody.IntegrateAcceleration(setDT);

		Jump();

		rigidbody.IntegrateVelocity(setDT);

		dtOffset -= setDT;
	}

	rigidbody.ClearForce();

	camera->SetPosition(rigidbody.GetPosition() + camOffset);
}

void Player::CollisionDetection(World* world) {

	glm::ivec3 checkPositions[] = {
		// Top
		glm::ivec3(0, -2, 0),
		glm::ivec3(1, -2, 0),
		glm::ivec3(0, -2, -1),
		glm::ivec3(-1, -2, 0),
		glm::ivec3(0, -2, 1),

		// Bottom
		glm::ivec3(0, 1, 0),
		glm::ivec3(1, 1, 0),
		glm::ivec3(0, 1, -1),
		glm::ivec3(-1, 1, 0),
		glm::ivec3(0, 1, 1),

		// Lower Middle
		glm::ivec3(1, -1, 0),
		glm::ivec3(0, -1, -1),
		glm::ivec3(-1, -1, 0),
		glm::ivec3(0, -1, 1),

		// Higher Middle
		glm::ivec3(1, 0, 0),
		glm::ivec3(0, 0, -1),
		glm::ivec3(-1, 0, 0),
		glm::ivec3(0, 0, 1),
	};

	CollisionDetection::ContactPoint contactPoint;

	for (int i = 0; i < 18; ++i) {
		BlockAtlas::Block collidingBlock = world->GetBlockAtPosition(blockCoordinates + checkPositions[i]);

		if (!collidingBlock.isFauna 
			&& collidingBlock.type != BlockAtlas::AIR 
			&& CollisionDetection::AABBIntersection(boxCollider, rigidbody.GetPosition(), AABB(), blockCoordinates + checkPositions[i], contactPoint)) {
			
			ImpulseCollisionResolution(contactPoint);

			if (i <= 5) {
				grounded = true;
			}
		}
	}
}

 void Player::ImpulseCollisionResolution(CollisionDetection::ContactPoint& point) {

	if (rigidbody.GetInverseMass() == 0) {
		return;
	}

	rigidbody.SetPosition(rigidbody.GetPosition() - point.normal * point.penetration);

	point.normal *= -1;
	float impulseForce = glm::dot(rigidbody.GetLinearVelocity(), point.normal);
	//float j = (1.0f + rigidbody.GetElasticity()) * impulseForce / rigidbody.GetInverseMass();
	glm::vec3 fullImpulse = point.normal * impulseForce;

	rigidbody.SetLinearVelocity(rigidbody.GetLinearVelocity() - fullImpulse);
}

 void Player::Jump() {
	 if (grounded && jumpCooldownTimer > jumpCooldown && !flyingCamMode && input->GetKeyDown(Input::KeyVal::SPACE)) {
		 rigidbody.ApplyLinearImpulse(glm::vec3(0, 1, 0) * jumpVelocity);
		 jumpCooldownTimer = 0;
		 grounded = false;
	 }
 }

BlockAtlas::Block Player::GetNearestBlock(World* world, glm::ivec3& collisionNormal) {
	Ray ray(camera->GetPosition(), camera->ForwardVector());

	float smallestDist = FLT_MAX;
	Ray::Collision col;
	BlockAtlas::Block nearestBlock = world->GetBlockAtPosition(blockCoordinates);

	int closestY = std::max(6, std::min(blockCoordinates.y, 249));
	for (int x = blockCoordinates.x - 6; x < blockCoordinates.x + 7; ++x) {
		for (int y = closestY - 6; y < closestY + 7; ++y) {
			for (int z = blockCoordinates.z - 6; z < blockCoordinates.z + 7; ++z) {
				BlockAtlas::Block block = world->GetBlockAtPosition(glm::ivec3(x, y, z));
				if (block.type != BlockAtlas::Type::AIR && CollisionDetection::RayAABBIntersection(ray, glm::vec3(x, y, z), glm::vec3(0.5, 0.5, 0.5), col)) {
					if (col.rayDistance < smallestDist) {
						smallestDist = col.rayDistance;
						collisionNormal = col.normal;
						block.position = glm::vec3(x, y, z);
						nearestBlock = block;
					}
				}
			}
		}
	}

	return nearestBlock;
}
