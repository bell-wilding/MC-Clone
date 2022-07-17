#include "World.h"

#include <iostream>

World::World(int seed, int initDimensions) {
	noise = new siv::BasicPerlinNoise<float>(seed);

	worldExtents.x = initDimensions / 2 - initDimensions;
	worldExtents.y = initDimensions / 2;
	// Same number of chunks on X and Z axis initially
	worldExtents.z = worldExtents.x;
	worldExtents.w = worldExtents.y;

	int i = 0;
	for (int x = worldExtents.x; x < worldExtents.y; ++x) {
		for (int z = worldExtents.z; z < worldExtents.w; ++z) {
			chunkMap[glm::ivec2(x, z)] = new Chunk(glm::vec3(x * 16, 0, z * 16));
			chunkMap[glm::ivec2(x, z)]->GenerateChunkData(seed, noise);
			chunkMap[glm::ivec2(x, z)]->InitialiseBuffers();
			++i;
		}
	}

	i = 0;
	for (int x = worldExtents.x; x < worldExtents.y; ++x) {
		for (int z = worldExtents.z; z < worldExtents.w; ++z) {
			chunkMap[glm::ivec2(x, z)]->CreateMesh(blockAtlas, chunkMap);
			++i;
			std::cout << "Chunks Generated: " << i << std::endl;
		}
	}
}

World::~World() {
	delete noise;
	for (int x = worldExtents.x; x < worldExtents.y; ++x) {
		for (int z = worldExtents.z; z < worldExtents.w; ++z) {
			delete chunkMap[glm::ivec2(x, z)];
			chunkMap[glm::ivec2(x, z)] = nullptr;
		}
	}
}

void World::Update(glm::vec2 playerChunk) {

}

Chunk* World::GetChunkAtPosition(glm::ivec2 chunkPosition) {
	return chunkMap[chunkPosition];
}

void World::PlaceBlock(glm::ivec3 position, BlockAtlas::Type type) {
	glm::ivec2 chunkCoords(std::floor(position.x / 16.0f), std::floor(position.z / 16.0f));
	glm::ivec2 chunkBlockCoords(position.x % 16, position.z % 16);
	glm::ivec3 blockCoords((chunkBlockCoords.x < 0 ? 16 + chunkBlockCoords.x : chunkBlockCoords.x), position.y, chunkBlockCoords.y < 0 ? 16 + chunkBlockCoords.y : chunkBlockCoords.y);
	Chunk* c = chunkMap[chunkCoords];
	if (c) {
		c->GetBlockAtPosition(blockCoords).type = type;
		c->GetBlockAtPosition(blockCoords).isTransparent = type == BlockAtlas::Type::LEAF || type == BlockAtlas::Type::WINDOW || type == BlockAtlas::Type::TALL_GRASS || type == BlockAtlas::Type::RED_FLOWER || type == BlockAtlas::Type::YELLOW_FLOWER || type == BlockAtlas::Type::SAPLING || type == BlockAtlas::Type::RED_MUSHROOM || type == BlockAtlas::Type::BROWN_MUSHROOM;
		c->GetBlockAtPosition(blockCoords).isFauna = type == BlockAtlas::Type::TALL_GRASS|| type == BlockAtlas::Type::RED_FLOWER || type == BlockAtlas::Type::YELLOW_FLOWER || type == BlockAtlas::Type::SAPLING || type == BlockAtlas::Type::RED_MUSHROOM || type == BlockAtlas::Type::BROWN_MUSHROOM;
		c->CreateMesh(blockAtlas, chunkMap);

		if (chunkMap[glm::vec2(chunkCoords.x + 1, chunkCoords.y)])
			chunkMap[glm::vec2(chunkCoords.x + 1, chunkCoords.y)]->CreateMesh(blockAtlas, chunkMap);

		if (chunkMap[glm::vec2(chunkCoords.x - 1, chunkCoords.y)])
			chunkMap[glm::vec2(chunkCoords.x - 1, chunkCoords.y)]->CreateMesh(blockAtlas, chunkMap);

		if (chunkMap[glm::vec2(chunkCoords.x, chunkCoords.y + 1)])
			chunkMap[glm::vec2(chunkCoords.x, chunkCoords.y + 1)]->CreateMesh(blockAtlas, chunkMap);

		if (chunkMap[glm::vec2(chunkCoords.x, chunkCoords.y - 1)])
			chunkMap[glm::vec2(chunkCoords.x, chunkCoords.y - 1)]->CreateMesh(blockAtlas, chunkMap);
	}
}

void World::DestroyBlock(glm::ivec3 position) {
	glm::ivec2 chunkCoords(std::floor(position.x / 16.0f), std::floor(position.z / 16.0f));
	glm::ivec2 chunkBlockCoords(position.x % 16, position.z % 16);
	glm::ivec3 blockCoords((chunkBlockCoords.x < 0 ? 16 + chunkBlockCoords.x : chunkBlockCoords.x), position.y, chunkBlockCoords.y < 0 ? 16 + chunkBlockCoords.y : chunkBlockCoords.y);
	Chunk* c = chunkMap[chunkCoords];
	if (c) {
		c->GetBlockAtPosition(blockCoords).type = BlockAtlas::Type::AIR;
		c->GetBlockAtPosition(blockCoords).isTransparent = true;
		c->GetBlockAtPosition(blockCoords).isFauna = false;
		c->CreateMesh(blockAtlas, chunkMap);

		if (chunkMap[glm::vec2(chunkCoords.x + 1, chunkCoords.y)])
			chunkMap[glm::vec2(chunkCoords.x + 1, chunkCoords.y)]->CreateMesh(blockAtlas, chunkMap);

		if (chunkMap[glm::vec2(chunkCoords.x - 1, chunkCoords.y)])
			chunkMap[glm::vec2(chunkCoords.x - 1, chunkCoords.y)]->CreateMesh(blockAtlas, chunkMap);

		if (chunkMap[glm::vec2(chunkCoords.x, chunkCoords.y + 1)])
			chunkMap[glm::vec2(chunkCoords.x, chunkCoords.y + 1)]->CreateMesh(blockAtlas, chunkMap);

		if (chunkMap[glm::vec2(chunkCoords.x, chunkCoords.y - 1)])
			chunkMap[glm::vec2(chunkCoords.x, chunkCoords.y - 1)]->CreateMesh(blockAtlas, chunkMap);
	}
}

BlockAtlas::Block World::GetBlockAtPosition(glm::ivec3 position) {
	if (position.y < 0 || position.y > 255) return BlockAtlas::Block{ BlockAtlas::Type::AIR };
	glm::ivec2 chunkCoords(std::floor(position.x / 16.0f), std::floor(position.z / 16.0f));
	glm::ivec2 chunkBlockCoords(position.x % 16, position.z % 16);
	glm::ivec3 blockCoords((chunkBlockCoords.x < 0 ? 16 + chunkBlockCoords.x : chunkBlockCoords.x), position.y, chunkBlockCoords.y < 0 ? 16 + chunkBlockCoords.y : chunkBlockCoords.y);
	return chunkMap[chunkCoords] ? chunkMap[chunkCoords]->GetBlockAtPosition(blockCoords) : BlockAtlas::Block{ BlockAtlas::Type::AIR };
}

//void CreateNewChunk(Chunk* chunk, glm::vec2 playerChunkPos, std::unordered_map<glm::ivec2, Chunk*> world, BlockAtlas blockAtlas, FastNoiseLite* noise) {
//
//	chunk = new Chunk(glm::vec3(playerChunkPos.x * 16, 0, playerChunkPos.y * 16), *noise);
//	chunk->GenerateChunkData(glm::vec3(playerChunkPos.x * 16, 0, playerChunkPos.y * 16), *noise);
//	chunk->GenerateMeshData(blockAtlas, world);
//
//	//if (world[glm::vec2(playerChunkPos.x + 1, playerChunkPos.y)])
//	//	world[glm::vec2(playerChunkPos.x + 1, playerChunkPos.y)]->CreateMesh(blockAtlas, world);
//
//	//if (world[glm::vec2(playerChunkPos.x - 1, playerChunkPos.y)])
//	//	world[glm::vec2(playerChunkPos.x - 1, playerChunkPos.y)]->CreateMesh(blockAtlas, world);
//
//	//if (world[glm::vec2(playerChunkPos.x, playerChunkPos.y + 1)])
//	//	world[glm::vec2(playerChunkPos.x, playerChunkPos.y + 1)]->CreateMesh(blockAtlas, world);
//
//	//if (world[glm::vec2(playerChunkPos.x, playerChunkPos.y - 1)])
//	//	world[glm::vec2(playerChunkPos.x, playerChunkPos.y - 1)]->CreateMesh(blockAtlas, world);
//}
