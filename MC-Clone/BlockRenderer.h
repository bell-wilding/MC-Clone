#pragma once

#include "BlockAtlas.h"

class BlockRenderer {
public:
	BlockRenderer();
	~BlockRenderer();

	void DrawBlock(BlockAtlas::Type type, glm::ivec3 position);

protected:
	unsigned int vao;
	unsigned int vertexBuffer;
	unsigned int indexBuffer;
};