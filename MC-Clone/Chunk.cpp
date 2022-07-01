#include "Chunk.h"
#include <iostream>

Chunk::Chunk(const glm::vec3 centerPos, FastNoiseLite& perlin) : centerPosition(centerPos) {

	for (int x = 0; x < 16; ++x) {
		for (int z = 0; z < 16; ++z) {
			float yPos = (perlin.GetNoise(centerPosition.x - 7.5f + x, centerPosition.z - 7.5f + z) + 1) * 0.5f * 255;
			int y = (int)yPos;

			blockData[x][y][z] = { BlockAtlas::Type::GRASS,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + y, centerPosition.z - 7.5f + z) };
			numBlocks++;

			for (int nextY = y - 1; nextY > -1; --nextY) {

				bool shouldDraw = false;

				if (x == 0 || x == 15 || z == 0 || z == 15) {
					shouldDraw = true;
				}

				if (nextY < 255 && blockData[x][nextY + 1][z].type == BlockAtlas::Type::GRASS) {
					shouldDraw = true;
				}

				if (shouldDraw) {
					if (nextY < y - 1)
						blockData[x][nextY][z] = { BlockAtlas::Type::STONE,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + nextY, centerPosition.z - 7.5f + z) };
					else
						blockData[x][nextY][z] = { BlockAtlas::Type::DIRT,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + nextY, centerPosition.z - 7.5f + z) };

					numBlocks++;
				}
				else {
					blockData[x][nextY][z] = { BlockAtlas::Type::AIR,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + nextY, centerPosition.z - 7.5f + z) };
				}
			}

			for (int nextY = y + 1; nextY < 256; ++nextY) {
				blockData[x][nextY][z] = { BlockAtlas::Type::AIR,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + nextY, centerPosition.z - 7.5f + z) };
			}
		}
	}
}

void Chunk::GetBlocks(BlockAtlas::Block blocks[16][256][16]) {
	for (int x = 0; x < 16; ++x) {
		for (int y = 0; y < 256; ++y) {
			for (int z = 0; z < 16; ++z) {
				blocks[x][y][z] = blockData[x][y][z];
			}
		}
	}
}

std::vector<BlockAtlas::Block> Chunk::GetBlocks() {
	std::vector<BlockAtlas::Block> blocks;
	for (int x = 0; x < 16; ++x) {
		for (int y = 0; y < 256; ++y) {
			for (int z = 0; z < 16; ++z) {
				blocks.push_back(blockData[x][y][z]);
			}
		}
	}

	return blocks;
}
