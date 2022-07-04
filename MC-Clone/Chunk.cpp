#include "Chunk.h"

#include <GL/glew.h>
#include <random>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include <iostream>

Chunk::Chunk(const glm::vec3 centerPos, FastNoiseLite& perlin) : centerPosition(centerPos) {
	chunkCoords = glm::vec2(centerPos.x / 16, centerPos.z / 16);

	for (int x = 0; x < 16; ++x) {
		for (int z = 0; z < 16; ++z) {
			float yPos = (perlin.GetNoise(centerPosition.x - 7.5f + x, centerPosition.z - 7.5f + z) + 1) * 0.5f * 255;
			int y = (int)yPos;

			blockData[x][y][z] = { BlockAtlas::Type::GRASS,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + y, centerPosition.z - 7.5f + z), false };
			numBlocks++;

			for (int nextY = y - 1; nextY > -1; --nextY) {

				if (nextY < y - 1)
					blockData[x][nextY][z] = { BlockAtlas::Type::STONE,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + nextY, centerPosition.z - 7.5f + z), false };
				else
					blockData[x][nextY][z] = { BlockAtlas::Type::DIRT,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + nextY, centerPosition.z - 7.5f + z), false };

				numBlocks++;
			}

			for (int nextY = y + 1; nextY < 256; ++nextY) {
				blockData[x][nextY][z] = { BlockAtlas::Type::AIR,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + nextY, centerPosition.z - 7.5f + z), true };
			}
		}
	}

	for (int x = 0; x < 16; ++x) {
		for (int z = 0; z < 16; ++z) {
			if (((double)rand() / (RAND_MAX)) < 0.02 && x > 2 && x < 14 && z > 2 && z < 14) {
				float yPos = (perlin.GetNoise(centerPosition.x - 7.5f + x, centerPosition.z - 7.5f + z) + 1) * 0.5f * 255;
				int y = (int)yPos;
				PlaceTree(x, y + 1, z);
			}
		}
	}
}

Chunk::~Chunk() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &vao);
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

void Chunk::Bind() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
}

void Chunk::CreateMesh(BlockAtlas& blockAtlas, std::unordered_map<glm::ivec2, Chunk*>& world) {

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
	xNeg.normal = glm::vec3(1, 0, 0);

	CubeFace xPos;
	xPos.vertices = {
	  glm::vec3(0.5f, -0.5f,  0.5f),
	  glm::vec3(0.5f, -0.5f, -0.5f),
	  glm::vec3(0.5f,  0.5f,  0.5f),
	  glm::vec3(0.5f,  0.5f, -0.5f)
	};
	xPos.indices = { 0,  1,  2,  1,  3,  2 };
	xPos.normal = glm::vec3(-1, 0, 0);

	CubeFace zNeg;
	zNeg.vertices = {
	  glm::vec3(0.5f, -0.5f, -0.5f),
	  glm::vec3(-0.5f, -0.5f, -0.5f),
	  glm::vec3(0.5f,  0.5f, -0.5f),
	  glm::vec3(-0.5f,  0.5f, -0.5f)
	};
	zNeg.indices = { 0,  1,  2,  1,  3,  2 };
	zNeg.normal = glm::vec3(0, 0, 1);

	CubeFace zPos;
	zPos.vertices = {
	  glm::vec3(-0.5f, -0.5f,  0.5f),
	  glm::vec3(0.5f, -0.5f,  0.5f),
	  glm::vec3(-0.5f,  0.5f,  0.5f),
	  glm::vec3(0.5f,  0.5f,  0.5f)
	};
	zPos.indices = { 0,  1,  2,  1,  3,  2 };
	zPos.normal = glm::vec3(0, 0, -1);

	CubeFace yNeg;
	yNeg.vertices = {
	  glm::vec3(-0.5f, -0.5f,  0.5f),
	  glm::vec3(-0.5f, -0.5f, -0.5f),
	  glm::vec3(0.5f, -0.5f,  0.5f),
	  glm::vec3(0.5f, -0.5f, -0.5f)
	};
	yNeg.indices = { 0,  1,  2,  1,  3,  2 };
	yNeg.normal = glm::vec3(0, 1, 0);

	CubeFace yPos;
	yPos.vertices = {
	  glm::vec3(-0.5f,  0.5f,  0.5f),
	  glm::vec3(-0.5f,  0.5f, -0.5f),
	  glm::vec3(0.5f,  0.5f,  0.5f),
	  glm::vec3(0.5f,  0.5f, -0.5f)
	};
	yPos.indices = { 2,  3,  1,  2,  1,  0 };
	yPos.normal = glm::vec3(0, -1, 0);

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

				BlockAtlas::Type blockType = blockData[blockPos.x][blockPos.y][blockPos.z].type;

				if (blockType != BlockAtlas::Type::AIR) {

					for (int i = 0; i < 6; ++i) {
						glm::ivec3 checkBlockPos = blockPos + checkDirections[i];

						bool shouldDraw = false;

						if (checkBlockPos.x == -1) {
							Chunk* c = world[glm::ivec2(chunkCoords.x - 1, chunkCoords.y)];
							if (c && c->GetBlockAtPosition(15, checkBlockPos.y, checkBlockPos.z).isTransparent) {
								shouldDraw = true;
							}
						}
						else if (checkBlockPos.x == 16) {
							Chunk* c = world[glm::ivec2(chunkCoords.x + 1, chunkCoords.y)];
							if (c && c->GetBlockAtPosition(0, checkBlockPos.y, checkBlockPos.z).isTransparent) {
								shouldDraw = true;
							}
						}
						else if (checkBlockPos.z == -1) {
							Chunk* c = world[glm::ivec2(chunkCoords.x, chunkCoords.y - 1)];
							if (c && c->GetBlockAtPosition(checkBlockPos.x, checkBlockPos.y, 15).isTransparent) {
								shouldDraw = true;
							}
						}
						else if (checkBlockPos.z == 16) {
							Chunk* c = world[glm::ivec2(chunkCoords.x, chunkCoords.y + 1)];
							if (c && c->GetBlockAtPosition(checkBlockPos.x, checkBlockPos.y, 0).isTransparent) {
								shouldDraw = true;
							}
						}
						else if (checkBlockPos.y != -1 && checkBlockPos.y != 256)  {
							shouldDraw = blockData[checkBlockPos.x][checkBlockPos.y][checkBlockPos.z].isTransparent;
						}

						if (shouldDraw) {
							CubeFace face = faceMap[checkDirections[i]];

							for (int j = 0; j < 4; ++j) {
								vertices.push_back(centerPosition.x + blockPos.x + face.vertices[j].x);
								vertices.push_back(centerPosition.y + blockPos.y + face.vertices[j].y);
								vertices.push_back(centerPosition.z + blockPos.z + face.vertices[j].z);
								glm::vec2 uvs = GetUVForVertex(j, i == 4 ? blockAtlas.GetBlockUVs(blockType).bottomUV : i == 5 ? blockAtlas.GetBlockUVs(blockType).topUV : blockAtlas.GetBlockUVs(blockType).sideUV);
								vertices.push_back(uvs.x);
								vertices.push_back(uvs.y);
								vertices.push_back(face.normal.x);
								vertices.push_back(face.normal.y);
								vertices.push_back(face.normal.z);
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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(5 * sizeof(float)));

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

BlockAtlas::Block Chunk::GetBlockAtPosition(int x, int y, int z) {
	return blockData[x][y][z];
}

glm::vec2 Chunk::GetUVForVertex(int vertIndex, glm::vec2 uvs) {
	if (vertIndex == 0) {
		return glm::vec2(uvs.x - 1, uvs.y - 1);
	}
	else if (vertIndex == 1) {
		return glm::vec2(uvs.x, uvs.y - 1);
	}
	else if (vertIndex == 2) {
		return glm::vec2(uvs.x - 1, uvs.y);
	}
	else if (vertIndex == 3) {
		return uvs;
	}
}

void Chunk::PlaceTree(int x, int y, int z) {

	for (int logPos = y; logPos < y + 5; ++logPos) {
		blockData[x][logPos][z] = { BlockAtlas::Type::WOOD_LOG,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + logPos, centerPosition.z - 7.5f + z), false };
		numBlocks++;
	}

	int topPos = y + 5;
	blockData[x][topPos][z] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + topPos, centerPosition.z - 7.5f + z), true };
	blockData[x+1][topPos][z] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x - 7.5f + x + 1, centerPosition.y - 127.5f + topPos, centerPosition.z - 7.5f + z), true };
	blockData[x-1][topPos][z] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x - 7.5f + x - 1, centerPosition.y - 127.5f + topPos, centerPosition.z - 7.5f + z), true };
	blockData[x][topPos][z+1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + topPos, centerPosition.z - 7.5f + z + 1), true };
	blockData[x][topPos][z-1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + topPos, centerPosition.z - 7.5f + z - 1), true };

	numBlocks += 5;

	topPos -= 1;
	for (int leafPos = x + 1; leafPos < x + 2; ++leafPos) {
		for (int leafPosY = topPos; leafPosY > topPos - 2; --leafPosY) {
			for (int leafPosTwo = z - 1; leafPosTwo < z + 2; ++leafPosTwo) {
				blockData[leafPos][leafPosY][leafPosTwo] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x - 7.5f + leafPos, centerPosition.y - 127.5f + leafPosY, centerPosition.z - 7.5f + leafPosTwo), true };
				numBlocks++;
			}
		}
	}

	for (int leafPos = x - 1; leafPos > x - 2; --leafPos) {
		for (int leafPosY = topPos; leafPosY > topPos - 2; --leafPosY) {
			for (int leafPosTwo = z - 1; leafPosTwo < z + 2; ++leafPosTwo) {
				blockData[leafPos][leafPosY][leafPosTwo] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x - 7.5f + leafPos, centerPosition.y - 127.5f + leafPosY, centerPosition.z - 7.5f + leafPosTwo), true };
				numBlocks++;
			}
		}
	}

	blockData[x][topPos][z + 1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + topPos, centerPosition.z - 7.5f + z + 1), true };
	blockData[x][topPos-1][z + 1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + topPos - 1, centerPosition.z - 7.5f + z + 1), true };
	blockData[x][topPos][z - 1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + topPos, centerPosition.z - 7.5f + z - 1), true };
	blockData[x][topPos - 1][z - 1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x - 7.5f + x, centerPosition.y - 127.5f + topPos - 1, centerPosition.z - 7.5f + z - 1), true };

	numBlocks += 4;
}
