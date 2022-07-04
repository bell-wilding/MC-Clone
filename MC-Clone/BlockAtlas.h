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
		OAK_LOG,
		BIRCH_LOG,
		LEAF,
		SAND,
		COBBLESTONE,
		BEDROCK
	};

	struct BlockUV {
		glm::vec2 topUV;
		glm::vec2 bottomUV;
		glm::vec2 sideUV;
	};

	struct Block {
		Type type;
		glm::vec3 position;
		bool isTransparent;
	};

	BlockAtlas();
	~BlockAtlas();

	std::vector<float> GetBlockVertexArray(BlockAtlas::Type type);

	BlockUV GetBlockUVs(Type blockType);

protected:

	std::map<Type, std::vector<float>> vaMap;
	std::map<Type, BlockUV> uvMap;

};

