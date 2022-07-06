#pragma once

#include <map>
#include <glm/glm.hpp>
#include <vector>
#include <string>

class BlockAtlas {
public:
	enum Type {
		AIR,
		GRASS,
		STONE,
		DIRT,
		WOOD_PLANK,
		OAK_LOG,
		BIRCH_LOG,
		LEAF,
		SAND,
		COBBLESTONE,
		BEDROCK,
		WINDOW,
		TALL_GRASS,
		RED_FLOWER,
		YELLOW_FLOWER,
		SAPLING,
		RED_MUSHROOM,
		BROWN_MUSHROOM,
		SAD_COWBOY,
		WATER_TOP,
		WATER
	};

	struct BlockUV {
		glm::vec2 topUV;
		glm::vec2 bottomUV;
		glm::vec2 sideUV;
	};

	struct Block {
		Type type;
		glm::vec3 position;
		bool isTransparent = false;
		bool isFauna = false;
	};

	BlockAtlas();
	~BlockAtlas();

	std::vector<float> GetBlockVertexArray(BlockAtlas::Type type);

	std::string GetTypeName(Type type) { return names[type]; }

	BlockUV GetBlockUVs(Type blockType);

protected:

	std::map<Type, std::vector<float>> vaMap;
	std::map<Type, BlockUV> uvMap;

	std::string names[21];

};

