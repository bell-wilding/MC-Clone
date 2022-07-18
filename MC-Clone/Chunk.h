#pragma once

#include <glm/glm.hpp>
#include "BlockAtlas.h"
#include "PerlinNoise.hpp"
#include <unordered_map>

struct CubeFace {
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	glm::vec3 normal;
};

class Chunk {
public:
	Chunk(const glm::vec3 centerPos);
	~Chunk();

	void GetBlocks(BlockAtlas::Block blockData[16][256][16]);

	std::vector<BlockAtlas::Block> GetBlocks();

	int GetNumBlocks() const { return numBlocks; };

	std::vector<unsigned int> GetIndices() { return indices; }
	std::vector<float> GetVertices() { return vertices; }

	void Bind();

	void GenerateChunkData(int seed, siv::BasicPerlinNoise<float>* noise);
	void InitialiseBuffers();

	void GenerateMeshData(BlockAtlas& blockAtlas, std::unordered_map<glm::ivec2, Chunk*>& world);
	void BufferData();

	void CreateMesh(BlockAtlas& blockAtlas, std::unordered_map<glm::ivec2, Chunk*>& world);

	glm::vec2 GetChunkCoords() const { return chunkCoords; }

	BlockAtlas::Block& GetBlockAtPosition(glm::ivec3 position);

	bool HasMesh() { return hasMesh; }

protected:
	glm::vec2 GetUVForVertex(int vertIndex, glm::vec2 uvs);

	void Smoothen(siv::BasicPerlinNoise<float>* noise);
	int GetNumSurroundingSolidBlocks(int x, int y, int z);

	void PlaceTree(int x, int y, int z);

	glm::ivec2 chunkCoords;

	glm::vec3 centerPosition;

	BlockAtlas::Block blockData[16][256][16];

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	int numBlocks;

	unsigned int vao;
	unsigned int vertexBuffer;
	unsigned int indexBuffer;

	bool hasMesh;
};

