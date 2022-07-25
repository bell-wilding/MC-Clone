#include "Chunk.h"

#include <GL/glew.h>
#include <random>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include <random>

#include <iostream>

Chunk::Chunk(const glm::vec3 centerPos) : centerPosition(centerPos) {
	chunkCoords = glm::ivec2(centerPos.x / 16, centerPos.z / 16);
}

Chunk::~Chunk() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &vao);

	glDeleteBuffers(1, &waterVertexBuffer);
	glDeleteBuffers(1, &waterIndexBuffer);
	glDeleteVertexArrays(1, &waterVAO);
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

void Chunk::BindWater() {
	glBindVertexArray(waterVAO);
	glBindBuffer(GL_ARRAY_BUFFER, waterVertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waterIndexBuffer);
}

void Chunk::CreateMesh(BlockAtlas& blockAtlas, std::unordered_map<glm::ivec2, Chunk*>& world) {
	GenerateMeshData(blockAtlas, world);
	BufferData();
}

void Chunk::GenerateMeshData(BlockAtlas& blockAtlas, std::unordered_map<glm::ivec2, Chunk*>& world) {
	vertices.clear();
	indices.clear();

	waterVertices.clear();
	waterIndices.clear();

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

	CubeFace faunaBlock1;
	faunaBlock1.vertices = {
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f)
	};
	faunaBlock1.indices = { 2,  3,  1,  2,  1,  0,
							0,  1,  2,  1,  3,  2 };
	faunaBlock1.normal = glm::vec3(0, 0, 0);

	CubeFace faunaBlock2;
	faunaBlock2.vertices = {
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f)
	};
	faunaBlock2.indices = { 2,  3,  1,  2,  1,  0,
							0,  1,  2,  1,  3,  2 };
	faunaBlock2.normal = glm::vec3(0, 0, 0);

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
	int numWaterVertices = 0;

	for (int x = 0; x < 16; ++x) {
		for (int y = 0; y < 256; ++y) {
			for (int z = 0; z < 16; ++z) {

				glm::ivec3 blockPos(x, y, z);

				BlockAtlas::Block block = blockData[blockPos.x][blockPos.y][blockPos.z];

				if (block.isFauna) {

					for (int j = 0; j < 4; ++j) {
						vertices.push_back(centerPosition.x + blockPos.x + faunaBlock1.vertices[j].x);
						vertices.push_back(centerPosition.y + blockPos.y + faunaBlock1.vertices[j].y);
						vertices.push_back(centerPosition.z + blockPos.z + faunaBlock1.vertices[j].z);
						glm::vec2 uvs = GetUVForVertex(j, blockAtlas.GetBlockUVs(block.type).sideUV);
						vertices.push_back(uvs.x);
						vertices.push_back(uvs.y);
						vertices.push_back(faunaBlock1.normal.x);
						vertices.push_back(faunaBlock1.normal.y);
						vertices.push_back(faunaBlock1.normal.z);
						numVertices++;
					}

					for (unsigned int index : faunaBlock1.indices) {
						indices.push_back(numVertices - 4 + index);
					}

					for (int j = 0; j < 4; ++j) {
						vertices.push_back(centerPosition.x + blockPos.x + faunaBlock2.vertices[j].x);
						vertices.push_back(centerPosition.y + blockPos.y + faunaBlock2.vertices[j].y);
						vertices.push_back(centerPosition.z + blockPos.z + faunaBlock2.vertices[j].z);
						glm::vec2 uvs = GetUVForVertex(j, blockAtlas.GetBlockUVs(block.type).sideUV);
						vertices.push_back(uvs.x);
						vertices.push_back(uvs.y);
						vertices.push_back(faunaBlock2.normal.x);
						vertices.push_back(faunaBlock2.normal.y);
						vertices.push_back(faunaBlock2.normal.z);
						numVertices++;
					}

					for (unsigned int index : faunaBlock2.indices) {
						indices.push_back(numVertices - 4 + index);
					}
				}
				else if (block.type == BlockAtlas::Type::WATER_TOP) {
					CubeFace face = faceMap[checkDirections[5]];

					for (int j = 0; j < 4; ++j) {
						waterVertices.push_back(centerPosition.x + blockPos.x + face.vertices[j].x);
						waterVertices.push_back(centerPosition.y + blockPos.y + face.vertices[j].y - 0.15f);
						waterVertices.push_back(centerPosition.z + blockPos.z + face.vertices[j].z);
						glm::vec2 uvs = GetUVForVertex(j, blockAtlas.GetBlockUVs(block.type).topUV);
						waterVertices.push_back(uvs.x);
						waterVertices.push_back(uvs.y);
						waterVertices.push_back(face.normal.x);
						waterVertices.push_back(face.normal.y);
						waterVertices.push_back(face.normal.z);
						numWaterVertices++;
					}

					for (unsigned int index : face.indices) {
						waterIndices.push_back(numWaterVertices - 4 + index);
					}
				}
				else if (block.type != BlockAtlas::Type::AIR && block.type != BlockAtlas::Type::WATER) {

					for (int i = 0; i < 6; ++i) {
						glm::ivec3 checkBlockPos = blockPos + checkDirections[i];

						bool shouldDraw = false;

						if (checkBlockPos.x == -1) {
							Chunk* c = world[glm::ivec2(chunkCoords.x - 1, chunkCoords.y)];
							if (c && c->GetBlockAtPosition(glm::ivec3(15, checkBlockPos.y, checkBlockPos.z)).isTransparent) {
								shouldDraw = true;
							}
						}
						else if (checkBlockPos.x == 16) {
							Chunk* c = world[glm::ivec2(chunkCoords.x + 1, chunkCoords.y)];
							if (c && c->GetBlockAtPosition(glm::ivec3(0, checkBlockPos.y, checkBlockPos.z)).isTransparent) {
								shouldDraw = true;
							}
						}
						else if (checkBlockPos.z == -1) {
							Chunk* c = world[glm::ivec2(chunkCoords.x, chunkCoords.y - 1)];
							if (c && c->GetBlockAtPosition(glm::ivec3(checkBlockPos.x, checkBlockPos.y, 15)).isTransparent) {
								shouldDraw = true;
							}
						}
						else if (checkBlockPos.z == 16) {
							Chunk* c = world[glm::ivec2(chunkCoords.x, chunkCoords.y + 1)];
							if (c && c->GetBlockAtPosition(glm::ivec3(checkBlockPos.x, checkBlockPos.y, 0)).isTransparent) {
								shouldDraw = true;
							}
						}
						else {
							shouldDraw = checkBlockPos.y == -1 || checkBlockPos.y == 256 || blockData[checkBlockPos.x][checkBlockPos.y][checkBlockPos.z].isTransparent;
						}

						if (shouldDraw) {
							CubeFace face = faceMap[checkDirections[i]];

							for (int j = 0; j < 4; ++j) {
								vertices.push_back(centerPosition.x + blockPos.x + face.vertices[j].x);
								vertices.push_back(centerPosition.y + blockPos.y + face.vertices[j].y);
								vertices.push_back(centerPosition.z + blockPos.z + face.vertices[j].z);
								glm::vec2 uvs = GetUVForVertex(j, i == 4 ? blockAtlas.GetBlockUVs(block.type).bottomUV : i == 5 ? blockAtlas.GetBlockUVs(block.type).topUV : blockAtlas.GetBlockUVs(block.type).sideUV);
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
}

void Chunk::BufferData() {
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(waterVAO);

	glBindBuffer(GL_ARRAY_BUFFER, waterVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, waterVertices.size() * sizeof(float), &waterVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waterIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, waterIndices.size() * sizeof(unsigned int), &waterIndices[0], GL_STATIC_DRAW);

	hasMesh = true;
}

BlockAtlas::Block& Chunk::GetBlockAtPosition(glm::ivec3 position) {
	return blockData[position.x][position.y][position.z];
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

void Chunk::Smoothen(siv::BasicPerlinNoise<float>* noise) {
	for (int x = 0; x < 16; ++x) {
		for (int z = 0; z < 16; ++z) {
			/*perlin.SetFrequency(0.3f);
			float p1 = (perlin.GetNoise(centerPosition.x + x, centerPosition.z + +z) + 1) * 0.5f;
			perlin.SetFrequency(0.0225f);
			float p2 = (perlin.GetNoise(centerPosition.x + x, centerPosition.z + +z) + 1) * 0.5f;
			perlin.SetFrequency(0.016875f);
			float p3 = (perlin.GetNoise(centerPosition.x + x, centerPosition.z + +z) + 1) * 0.5;

			float yPos = (p1 + p2 + p3) / 3 * 255;
			int y = (int)yPos;

			for (int nextY = y - 1; nextY > 0; --nextY) {

				int numSurroundingBlocks = GetNumSurroundingSolidBlocks(x, nextY, z);

				if (numSurroundingBlocks > 22) {
					blockData[x][nextY][z] = { BlockAtlas::Type::STONE,  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), false };
				} else if (numSurroundingBlocks < 22) {
					blockData[x][nextY][z] = { BlockAtlas::Type::AIR, glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), true };
					numBlocks--;
				}

			}*/
		}
	}
}

int Chunk::GetNumSurroundingSolidBlocks(int x, int y, int z) {
	int wallCount = 0;
	for (int neighbourX = x-1; neighbourX < x+2; ++neighbourX) {
		for (int neighbourY = y-1; neighbourY < y+2; ++neighbourY) {
			for (int neighbourZ = z-1; neighbourZ < z+2; ++neighbourZ) {
				if ((neighbourX > -1 && neighbourX < 16 && neighbourY > -1 && neighbourY < 256 && neighbourZ > -1 && neighbourZ < 16)) {
					if ((neighbourX != x || neighbourY != y || neighbourZ != z)) {
						if (GetBlockAtPosition(glm::ivec3(neighbourX, neighbourY, neighbourZ)).type != BlockAtlas::Type::AIR) {
							wallCount++;
						}
					}
				}
				else {
					wallCount++;
				}
			}
		}
	}
	return wallCount;
}

void Chunk::GenerateChunkData(int seed, siv::BasicPerlinNoise<float>* noise) {

	//srand(seed);

	for (int x = 0; x < 16; ++x) {
		for (int z = 0; z < 16; ++z) {

			float yPos = noise->normalizedOctave2D_01((centerPosition.x + x) * 0.003f, (centerPosition.z + z) * 0.003f, 8) * 255;
			int y = (int)yPos;

			blockData[x][y][z] = { BlockAtlas::Type::GRASS,  glm::vec3(centerPosition.x + x, centerPosition.y + y, centerPosition.z + z), false };
			numBlocks++;

			for (int nextY = y - 1; nextY > -1; --nextY) {

				if (nextY == 0) {
					blockData[x][nextY][z] = { BlockAtlas::Type::BEDROCK,  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), false };
				}
				else if (nextY < y - 1) {
					float val = noise->normalizedOctave3D_01((centerPosition.x + x) * 0.15f, (centerPosition.z + z) * 0.15f, (centerPosition.y + nextY) * 0.15f, 2);

					if (val > 0.775 && nextY < 20) {
						blockData[x][nextY][z] = { BlockAtlas::Type::DIAMOND_ORE,  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), false };
					}
					else if (val > 0.75 && nextY < 40) {
						blockData[x][nextY][z] = { BlockAtlas::Type::GOLD_ORE,  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), false };
					}
					else if (val > 0.7 && nextY < 70) {
						blockData[x][nextY][z] = { BlockAtlas::Type::IRON_ORE,  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), false };
					}
					else if (val > 0.65 && nextY < 100) {
						blockData[x][nextY][z] = { BlockAtlas::Type::COAL_ORE,  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), false };
					}
					else {
						blockData[x][nextY][z] = { BlockAtlas::Type::STONE,  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), false };
					}
				}
				else {
					blockData[x][nextY][z] = { (rand() % 2 == 0 ? BlockAtlas::Type::STONE : BlockAtlas::Type::DIRT),  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), false };
				}

				numBlocks++;
			}

			for (int nextY = y + 1; nextY < 256; ++nextY) {
				if (nextY > 70 && nextY < 80) {
					blockData[x][nextY][z] = { BlockAtlas::Type::WATER,  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), true };
				} else if (nextY == 80) {
					blockData[x][nextY][z] = { BlockAtlas::Type::WATER_TOP,  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), true };
				}
				else {
					blockData[x][nextY][z] = { BlockAtlas::Type::AIR,  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), true };
				}
			}
		}
	}

	float rVal = ((double)rand() / (RAND_MAX));
	bool entrance = rVal < 0.025;

	for (int x = 0; x < 16; ++x) {
		for (int z = 0; z < 16; ++z) {
			float yPos = noise->normalizedOctave2D_01((centerPosition.x + x) * 0.003f, (centerPosition.z + z) * 0.003f, 8) * 255;
			int y = (int)yPos;

			bool destroyBlockAbove = false;

			for (int nextY = entrance ? y : y - 2; nextY > 0; --nextY) {
				/*float val = noise->normalizedOctave3D_01((centerPosition.x + x) * 0.025f, (centerPosition.z + z) * 0.025f, (centerPosition.y + nextY) * 0.03f, 4, 0.75);
				if (nextY <= y - 2) {
					if (val < 0.41|| val > 0.59) blockData[x][nextY][z] = { BlockAtlas::Type::AIR,  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), true };
				}
				else {
					if (val < 0.3 || val > 0.7) {
						blockData[x][nextY][z] = { BlockAtlas::Type::AIR,  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), true };
						destroyBlockAbove = true;
					}
				}*/
				float val = noise->normalizedOctave3D_01((centerPosition.x + x) * 0.008f, (centerPosition.z + z) * 0.008f, (centerPosition.y + nextY) * 0.009f, 5, 5.0);
				if (nextY <= y - 2) {
					if (val < 0.43 || val > 0.57) blockData[x][nextY][z] = { BlockAtlas::Type::AIR,  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), true };
				}
				else {
					if (val < 0.35 || val > 0.65) {
						blockData[x][nextY][z] = { BlockAtlas::Type::AIR,  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), true };
						destroyBlockAbove = true;
					}
				}
			}

			if (destroyBlockAbove) {
				for (int nextY = y + 1; nextY < 256; ++nextY) {
					blockData[x][nextY][z] = { BlockAtlas::Type::AIR,  glm::vec3(centerPosition.x + x, centerPosition.y + nextY, centerPosition.z + z), true };
				}
			}
		}
	}


	for (int x = 0; x < 16; ++x) {
		for (int z = 0; z < 16; ++z) {

			float yPos = noise->normalizedOctave2D_01((centerPosition.x + x) * 0.003f, (centerPosition.z + z) * 0.003f, 8) * 255;
			int y = (int)yPos;
			if (GetBlockAtPosition(glm::ivec3(x, y, z)).type == BlockAtlas::Type::GRASS && GetBlockAtPosition(glm::ivec3(x, y + 1, z)).type != BlockAtlas::Type::WATER_TOP) {
				float randVal = ((double)rand() / (RAND_MAX));

				if (y != 255) {
					if (randVal < 0.5) {
						blockData[x][y + 1][z] = { BlockAtlas::Type::TALL_GRASS,  glm::vec3(centerPosition.x + x, centerPosition.y + y + 1, centerPosition.z + z), true, true };
						numBlocks++;
					}
					if (randVal < 0.15) {
						blockData[x][y + 1][z] = { BlockAtlas::Type::RED_FLOWER,  glm::vec3(centerPosition.x + x, centerPosition.y + y + 1, centerPosition.z + z), true, true };
					}
					if (randVal < 0.1) {
						blockData[x][y + 1][z] = { BlockAtlas::Type::YELLOW_FLOWER,  glm::vec3(centerPosition.x + x, centerPosition.y + y + 1, centerPosition.z + z), true, true };
					}
					if (randVal < 0.05) {
						blockData[x][y + 1][z] = { BlockAtlas::Type::RED_MUSHROOM,  glm::vec3(centerPosition.x + x, centerPosition.y + y + 1, centerPosition.z + z), true, true };
					}
					if (randVal < 0.03) {
						blockData[x][y + 1][z] = { BlockAtlas::Type::SAPLING,  glm::vec3(centerPosition.x + x, centerPosition.y + y + 1, centerPosition.z + z), true, true };
					}
					if (randVal < 0.01) {
						blockData[x][y + 1][z] = { BlockAtlas::Type::BROWN_MUSHROOM,  glm::vec3(centerPosition.x + x, centerPosition.y + y + 1, centerPosition.z + z), true, true };
					}
				}
			}
		}
	}


	for (int x = 0; x < 16; ++x) {
		for (int z = 0; z < 16; ++z) {
			if (((double)rand() / (RAND_MAX)) < 0.01 && x > 2 && x < 14 && z > 2 && z < 14) {
				float yPos = noise->normalizedOctave2D_01((centerPosition.x + x) * 0.003f, (centerPosition.z + z) * 0.003f, 8) * 255;
				int y = (int)yPos;
				if (GetBlockAtPosition(glm::ivec3(x, y, z)).type == BlockAtlas::Type::GRASS) {
					PlaceTree(x, y + 1, z);
				}
			}
		}
	}
}

void Chunk::InitialiseBuffers() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(5 * sizeof(float)));

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glGenVertexArrays(1, &waterVAO);
	glBindVertexArray(waterVAO);

	glGenBuffers(1, &waterVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, waterVertexBuffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(5 * sizeof(float)));

	glGenBuffers(1, &waterIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waterIndexBuffer);
}

void Chunk::PlaceTree(int x, int y, int z) {

	bool birch = ((double)rand() / (RAND_MAX)) < 0.3;

	for (int logPos = y; logPos < y + 5; ++logPos) {
		blockData[x][logPos][z] = { birch ? BlockAtlas::Type::BIRCH_LOG : BlockAtlas::Type::OAK_LOG,  glm::vec3(centerPosition.x + x, centerPosition.y + logPos, centerPosition.z + z), false };
		numBlocks++;
	}

	int topPos = y + 6;
	blockData[x][topPos][z] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos, centerPosition.z + z), true };
	blockData[x+1][topPos][z] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x + 1, centerPosition.y + topPos, centerPosition.z + z), true };
	blockData[x-1][topPos][z] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x - 1, centerPosition.y + topPos, centerPosition.z + z), true };
	blockData[x][topPos][z+1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos, centerPosition.z + z + 1), true };
	blockData[x][topPos][z-1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos, centerPosition.z + z - 1), true };

	topPos--;
	blockData[x][topPos][z] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos, centerPosition.z + z), true };
	blockData[x + 1][topPos][z] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x + 1, centerPosition.y + topPos, centerPosition.z + z), true };
	blockData[x - 1][topPos][z] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x - 1, centerPosition.y + topPos, centerPosition.z + z), true };
	blockData[x][topPos][z + 1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos, centerPosition.z + z + 1), true };
	blockData[x][topPos][z - 1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos, centerPosition.z + z - 1), true };
	blockData[x + 1][topPos][z+1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x + 1, centerPosition.y + topPos, centerPosition.z + z + 1), true };
	blockData[x - 1][topPos][z-1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x - 1, centerPosition.y + topPos, centerPosition.z + z - 1), true };
	blockData[x-1][topPos][z + 1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos, centerPosition.z + z + 1), true };
	blockData[x+1][topPos][z - 1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos, centerPosition.z + z - 1), true };

	numBlocks += 13;

	topPos--;
	for (int leafPos = x + 1; leafPos < x + 3; ++leafPos) {
		for (int leafPosY = topPos; leafPosY > topPos - 2; --leafPosY) {
			for (int leafPosTwo = z - 2; leafPosTwo < z + 3; ++leafPosTwo) {
				blockData[leafPos][leafPosY][leafPosTwo] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + leafPos, centerPosition.y + leafPosY, centerPosition.z + leafPosTwo), true };
				numBlocks++;
			}
		}
	}

	for (int leafPos = x - 1; leafPos > x - 3; --leafPos) {
		for (int leafPosY = topPos; leafPosY > topPos - 2; --leafPosY) {
			for (int leafPosTwo = z - 2; leafPosTwo < z + 3; ++leafPosTwo) {
				blockData[leafPos][leafPosY][leafPosTwo] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + leafPos, centerPosition.y + leafPosY, centerPosition.z + leafPosTwo), true };
				numBlocks++;
			}
		}
	}

	blockData[x][topPos][z + 1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos, centerPosition.z + z + 1), true };
	blockData[x][topPos-1][z + 1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos - 1, centerPosition.z + z + 1), true };
	blockData[x][topPos][z - 1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos, centerPosition.z + z - 1), true };
	blockData[x][topPos - 1][z - 1] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos - 1, centerPosition.z + z - 1), true };
	blockData[x][topPos][z + 2] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos, centerPosition.z + z + 1), true };
	blockData[x][topPos - 1][z + 2] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos - 1, centerPosition.z + z + 1), true };
	blockData[x][topPos][z - 2] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos, centerPosition.z + z - 1), true };
	blockData[x][topPos - 1][z - 2] = { BlockAtlas::Type::LEAF,  glm::vec3(centerPosition.x + x, centerPosition.y + topPos - 1, centerPosition.z + z - 1), true };

	numBlocks += 8;
}
