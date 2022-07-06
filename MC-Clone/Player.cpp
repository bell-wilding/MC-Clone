#include <GLFW/glfw3.h>

#include "Renderer.h"

#include "Player.h"

#include "Ray.h"
#include "CollisionDetection.h"
#include "World.h"

#include <algorithm>

Player::Player(PerspectiveCamera* cam) : camera(cam) {
	chunkCoordinates = glm::ivec2((camera->GetPosition().x - 8) / 16, (camera->GetPosition().z - 8) / 16);
	blockCoordinates = glm::ivec3(std::round(camera->GetPosition().x), std::round(camera->GetPosition().y), std::round(camera->GetPosition().z));

	activeBlockType = BlockAtlas::Type::GRASS;

	canBreakBlock = true;
	canPlaceBlock = true;
}

void Player::Update(float dt, GLFWwindow* window, World* world, Renderer& renderer) {

	chunkCoordinates = glm::ivec2((camera->GetPosition().x - 8) / 16, (camera->GetPosition().z - 8) / 16);
	blockCoordinates = glm::ivec3(std::round(camera->GetPosition().x), std::round(camera->GetPosition().y), std::round(camera->GetPosition().z));

	ChangeActiveBlockType(window);
	HandleBlockInteraction(window, world, renderer);
}

void Player::HandleBlockInteraction(GLFWwindow* window, World* world, Renderer& renderer) {
	glm::ivec3 collisionNormal;
	BlockAtlas::Block block = GetNearestBlock(world, collisionNormal);

	if (block.type != BlockAtlas::Type::AIR) {
		renderer.DrawBox(block.position, glm::vec3(0.505, 0.505, 0.505));
		if (canBreakBlock && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			world->DestroyBlock(block.position);
			canBreakBlock = false;
		}
		if (canPlaceBlock && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			world->PlaceBlock((glm::ivec3)block.position + collisionNormal, activeBlockType);
			canPlaceBlock = false;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		canBreakBlock = true;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		canPlaceBlock = true;
	}
}

void Player::ChangeActiveBlockType(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		activeBlockType = BlockAtlas::GRASS;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		activeBlockType = BlockAtlas::DIRT;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		activeBlockType = BlockAtlas::STONE;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		activeBlockType = BlockAtlas::OAK_LOG;
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
		activeBlockType = BlockAtlas::LEAF;
	}
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		activeBlockType = BlockAtlas::WOOD_PLANK;
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
		activeBlockType = BlockAtlas::WINDOW;
	}
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
		activeBlockType = BlockAtlas::SAD_COWBOY;
	}
}

BlockAtlas::Block Player::GetNearestBlock(World* world, glm::ivec3& collisionNormal) {
	Ray ray(camera->GetPosition(), camera->ForwardVector());

	float smallestDist = FLT_MAX;
	Ray::Collision col;
	BlockAtlas::Block nearestBlock = world->GetBlockAtPosition(blockCoordinates);

	blockCoordinates.y = std::max(10, std::min(blockCoordinates.y, 245));
	for (int x = blockCoordinates.x - 10; x < blockCoordinates.x + 11; ++x) {
		for (int y = blockCoordinates.y - 10; y < blockCoordinates.y + 11; ++y) {
			for (int z = blockCoordinates.z - 10; z < blockCoordinates.z + 11; ++z) {
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
