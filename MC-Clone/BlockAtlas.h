#pragma once

#include <map>
#include <glm/glm.hpp>
#include <vector>

class BlockAtlas {
public:
	enum Type {
		AIR,
		GRASS,
		STONE,
		DIRT,
		WOOD_PLANK,
		WOOD_LOG,
		SAND,
		COBBLESTONE
	};

	Type typeArray[8] = {
		AIR,
		GRASS,
		STONE,
		DIRT,
		WOOD_PLANK,
		WOOD_LOG,
		SAND,
		COBBLESTONE
	};

	struct BlockUV {
		glm::vec2 topUV;
		glm::vec2 bottomUV;
		glm::vec2 sideUV;
	};

	struct Block {
		Type type;
		glm::vec3 position;
	};

	BlockAtlas();
	~BlockAtlas();

	std::vector<float> GetBlockVertexArray(BlockAtlas::Type type);

protected:

	std::map<Type, std::vector<float>> vaMap;
	std::map<Type, BlockUV> uvMap;

};

