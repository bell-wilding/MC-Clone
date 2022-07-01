#pragma once

#include <glm/glm.hpp>
#include "BlockAtlas.h"
#include "FastNoiseLite.h"

class Chunk {
public:
	Chunk(const glm::vec3 centerPos, FastNoiseLite& perlin);
	~Chunk() {};

	void GetBlocks(BlockAtlas::Block blockData[16][256][16]);

	std::vector<BlockAtlas::Block> GetBlocks();

	int GetNumBlocks() const { return numBlocks; };

protected:
	glm::vec3 centerPosition;

	BlockAtlas::Block blockData[16][256][16];

	int numBlocks;
};

