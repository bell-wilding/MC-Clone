#pragma once

#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include <glm/glm.hpp>

#include "Chunk.h"

class World {
public:
	World(int seed, int initDimensions);
	~World();

	Chunk* GetChunkAtPosition(glm::ivec2 chunkPosition);
	BlockAtlas::Block GetBlockAtPosition(glm::ivec3 position);

	void PlaceBlock(glm::ivec3 position, BlockAtlas::Type type);
	void DestroyBlock(glm::ivec3 position);

	glm::ivec4 GetWorldExtents() const { return worldExtents; }

protected:

	std::unordered_map<glm::ivec2, Chunk*> chunkMap;
	BlockAtlas blockAtlas;
	FastNoiseLite* noise;

	glm::ivec4 worldExtents;
};