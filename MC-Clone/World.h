#pragma once

#include <unordered_map>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include <glm/glm.hpp>

#include "Chunk.h"

class World {
public:
	World(int seed, int worldDimensions);
	~World();

	void Update(glm::vec3 playerPos, glm::ivec2 playerChunk);

	Chunk* GetChunkAtPosition(glm::ivec2 chunkPosition);
	BlockAtlas::Block GetBlockAtPosition(glm::ivec3 position);

	void PlaceBlock(glm::ivec3 position, BlockAtlas::Type type);
	void DestroyBlock(glm::ivec3 position);

	glm::ivec4 GetWorldExtents() const { return worldExtents; }

	std::vector<glm::ivec2> GetChunksNearestToFarthest() { return chunksNearestToFarthest; }

protected:

	std::vector<glm::ivec2> chunksNearestToFarthest;
	std::unordered_map<glm::ivec2, Chunk*> chunkMap;
	BlockAtlas blockAtlas;
	siv::BasicPerlinNoise<float>* noise;

	glm::ivec4 worldExtents;
	int activeDimensions;

	int seed;

	int extents;
};