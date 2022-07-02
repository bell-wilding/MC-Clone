#pragma once

#include <glm/glm.hpp>
#include "BlockAtlas.h"
#include "FastNoiseLite.h"

struct CubeFace {
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
};

class Chunk {
public:
	Chunk(const glm::vec3 centerPos, FastNoiseLite& perlin);
	~Chunk() {};

	void GetBlocks(BlockAtlas::Block blockData[16][256][16]);

	std::vector<BlockAtlas::Block> GetBlocks();

	int GetNumBlocks() const { return numBlocks; };

	std::vector<unsigned int> GetIndices() const { return indices; }
	std::vector<glm::vec3> GetVertices() const { return vertices; }

protected:
	glm::vec3 centerPosition;

	BlockAtlas::Block blockData[16][256][16];

	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;

	int numBlocks;
};

