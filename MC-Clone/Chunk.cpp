#include "Chunk.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include <iostream>
#include <unordered_map>

Chunk::Chunk(const glm::vec3 centerPos, FastNoiseLite& perlin) : centerPosition(centerPos) {

	for (int x = 0; x < 16; ++x) {
		for (int z = 0; z < 16; ++z) {
			float yPos = (perlin.GetNoise(centerPosition.x - 7.5f + x, centerPosition.z - 7.5f + z) + 1) * 0.5f * 255;
			int y = (int)yPos;

			blockData[x][y][z] = { BlockAtlas::Type::GRASS,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + y, centerPosition.z - 7.5f + z) };
			numBlocks++;

			for (int nextY = y - 1; nextY > -1; --nextY) {

				if (nextY < y - 1)
					blockData[x][nextY][z] = { BlockAtlas::Type::STONE,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + nextY, centerPosition.z - 7.5f + z) };
				else
					blockData[x][nextY][z] = { BlockAtlas::Type::DIRT,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + nextY, centerPosition.z - 7.5f + z) };

				numBlocks++;
			}

			for (int nextY = y + 1; nextY < 256; ++nextY) {
				blockData[x][nextY][z] = { BlockAtlas::Type::AIR,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + nextY, centerPosition.z - 7.5f + z) };
			}
		}
	}

	glm::ivec3 checkDirections[] = {
		glm::ivec3(-1, 0, 0),
		glm::ivec3(1, 0, 0),
		glm::ivec3(0, 0, -1),
		glm::ivec3(0, 0, 1),
		glm::ivec3(0, -1, 0),
		glm::ivec3(0, 1, 0)
	};

	CubeFace xNeg;
	xNeg.vertices = {
	  glm::vec3(-0.5f, -0.5f, -0.5f),
	  glm::vec3(-0.5f, -0.5f,  0.5f),
	  glm::vec3(-0.5f,  0.5f, -0.5f),
	  glm::vec3(-0.5f,  0.5f,  0.5f)
	};
	xNeg.indices = { 0,  1,  2,  1,  3,  2 };

	CubeFace xPos;
	xPos.vertices = {
	  glm::vec3(0.5f, -0.5f,  0.5f),
	  glm::vec3(0.5f, -0.5f, -0.5f),
	  glm::vec3(0.5f,  0.5f,  0.5f),
	  glm::vec3(0.5f,  0.5f, -0.5f)
	};
	xPos.indices = { 0,  1,  2,  1,  3,  2 };

	CubeFace zNeg;
	zNeg.vertices = {
	  glm::vec3(0.5f, -0.5f, -0.5f),
	  glm::vec3(-0.5f, -0.5f, -0.5f),
	  glm::vec3(0.5f,  0.5f, -0.5f),
	  glm::vec3(-0.5f,  0.5f, -0.5f)
	};
	zNeg.indices = { 0,  1,  2,  1,  3,  2 };

	CubeFace zPos;
	zPos.vertices = {
	  glm::vec3(-0.5f, -0.5f,  0.5f),
	  glm::vec3(0.5f, -0.5f,  0.5f),
	  glm::vec3(-0.5f,  0.5f,  0.5f),
	  glm::vec3(0.5f,  0.5f,  0.5f)
	};
	zPos.indices = { 0,  1,  2,  1,  3,  2 };

	CubeFace yNeg;
	yNeg.vertices = {
	  glm::vec3(-0.5f, -0.5f,  0.5f),
	  glm::vec3(-0.5f, -0.5f, -0.5f),
	  glm::vec3(0.5f, -0.5f,  0.5f),
	  glm::vec3(0.5f, -0.5f, -0.5f)
	};
	yNeg.indices = { 0,  1,  2,  1,  3,  2 };

	CubeFace yPos;
	yPos.vertices = {
	  glm::vec3(-0.5f,  0.5f,  0.5f),
	  glm::vec3(-0.5f,  0.5f, -0.5f),
	  glm::vec3(0.5f,  0.5f,  0.5f),
	  glm::vec3(0.5f,  0.5f, -0.5f)
	};
	yPos.indices = { 0,  1,  2,  1,  3,  2 };

	CubeFace faces[6] = {
		xNeg,
		xPos,
		zNeg,
		zPos,
		yNeg,
		yPos
	};

	std::unordered_map<glm::ivec3, CubeFace> faceMap;

	for (int i = 0; i < 6; ++i) {
		faceMap[checkDirections[i]] = faces[i];
	}

	int numVertices = 0;

	for (int x = 0; x < 16; ++x) {
		for (int y = 0; y < 256; ++y) {
			for (int z = 0; z < 16; ++z) {

				glm::ivec3 blockPos(x, y, z);

				if (blockData[blockPos.x][blockPos.y][blockPos.z].type != BlockAtlas::Type::AIR && x > 0 && x < 15 && z > 0 && z < 15 && y > 0 && y < 255) {
					for (int i = 0; i < 6; ++i) {
						glm::ivec3 checkBlockPos = blockPos + checkDirections[i];

						if (blockData[checkBlockPos.x][checkBlockPos.y][checkBlockPos.z].type == BlockAtlas::Type::AIR) {
							CubeFace face = faceMap[checkDirections[i]];

							for (glm::vec3 vert : face.vertices) {
								vertices.push_back(blockPos.x + vert.x);
								vertices.push_back(blockPos.y + vert.y);
								vertices.push_back(blockPos.z + vert.z);
								numVertices++;
							}

							for (unsigned int index : face.indices) {
								indices.push_back(numVertices - 4 + index);
							}
						}
					}
				}
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
