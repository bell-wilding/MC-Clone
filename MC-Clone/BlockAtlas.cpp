#include "BlockAtlas.h"

BlockAtlas::BlockAtlas() {

	uvMap[BlockAtlas::Type::GRASS]			= { glm::vec2(1, 16), glm::vec2(3, 16), glm::vec2(4, 16) };
	uvMap[BlockAtlas::Type::OAK_LOG]		= { glm::vec2(6, 15), glm::vec2(6, 15), glm::vec2(5, 15) };
	uvMap[BlockAtlas::Type::BIRCH_LOG]		= { glm::vec2(6, 15), glm::vec2(6, 15), glm::vec2(6, 9) };
	uvMap[BlockAtlas::Type::COBBLESTONE]	= { glm::vec2(1, 15), glm::vec2(1, 15), glm::vec2(1, 15) };
	uvMap[BlockAtlas::Type::WOOD_PLANK]		= { glm::vec2(5, 16), glm::vec2(5, 16), glm::vec2(5, 16) };
	uvMap[BlockAtlas::Type::LEAF]			= { glm::vec2(5, 13), glm::vec2(5, 13), glm::vec2(5, 13) };
	uvMap[BlockAtlas::Type::STONE]			= { glm::vec2(2, 16), glm::vec2(2, 16), glm::vec2(2, 16) };
	uvMap[BlockAtlas::Type::DIRT]			= { glm::vec2(3, 16), glm::vec2(3, 16), glm::vec2(3, 16) };
	uvMap[BlockAtlas::Type::BEDROCK]		= { glm::vec2(2, 15), glm::vec2(2, 15), glm::vec2(2, 15) };
	uvMap[BlockAtlas::Type::WINDOW]			= { glm::vec2(2, 13), glm::vec2(2, 13), glm::vec2(2, 13) };
	uvMap[BlockAtlas::Type::TALL_GRASS]		= { glm::vec2(13, 10), glm::vec2(13, 10), glm::vec2(13, 10) };
	uvMap[BlockAtlas::Type::RED_FLOWER]		= { glm::vec2(13, 16), glm::vec2(13, 16), glm::vec2(13, 16) };
	uvMap[BlockAtlas::Type::YELLOW_FLOWER]	= { glm::vec2(14, 16), glm::vec2(14, 16), glm::vec2(14, 16) };
	uvMap[BlockAtlas::Type::SAPLING]		= { glm::vec2(16, 16), glm::vec2(16, 16), glm::vec2(16, 16) };
	uvMap[BlockAtlas::Type::RED_MUSHROOM]	= { glm::vec2(13, 15), glm::vec2(13, 15), glm::vec2(13, 15) };
	uvMap[BlockAtlas::Type::BROWN_MUSHROOM]	= { glm::vec2(14, 15), glm::vec2(14, 15), glm::vec2(14, 15) };
	uvMap[BlockAtlas::Type::SAD_COWBOY]		= { glm::vec2(14, 9), glm::vec2(14, 9), glm::vec2(14, 9) };
	uvMap[BlockAtlas::Type::WATER_TOP]		= { glm::vec2(14, 4), glm::vec2(14, 10), glm::vec2(14, 10) };
	uvMap[BlockAtlas::Type::WATER]			= { glm::vec2(14, 10), glm::vec2(14, 10), glm::vec2(14, 10) };

	names[0] = "Air";
	names[1] = "Grass";
	names[2] = "Stone";
	names[3] = "Dirt";
	names[4] = "Wood Plank";
	names[5] = "Oak Log";
	names[6] = "Birch Log";
	names[7] = "Leaf";
	names[8] = "Sand";
	names[9] = "Cobblestone";
	names[10] = "Bedrock";
	names[11] = "Window";
	names[12] = "Tall Grass";
	names[13] = "Red Glower";
	names[14] = "Yellow Flower";
	names[15] = "Sapling";
	names[16] = "Red Mushroom";
	names[17] = "Brown Mushroom";
	names[18] = "Sad Cowboy";

	for (const auto &uvInfo: uvMap) {

		std::vector<float> vertices = {
			// Face 1
		  -0.5f, -0.5f, -0.5f, uvInfo.second.sideUV.x - 1,	uvInfo.second.sideUV.y - 1,		-1.0, 0.0, 0.0,
		  -0.5f, -0.5f,  0.5f, uvInfo.second.sideUV.x,		uvInfo.second.sideUV.y - 1,		-1.0, 0.0, 0.0,
		  -0.5f,  0.5f, -0.5f, uvInfo.second.sideUV.x - 1,	uvInfo.second.sideUV.y,			-1.0, 0.0, 0.0,
		  -0.5f,  0.5f,  0.5f, uvInfo.second.sideUV.x,		uvInfo.second.sideUV.y,			-1.0, 0.0, 0.0,
		  // Face 2
		  -0.5f, -0.5f,  0.5f, uvInfo.second.sideUV.x - 1,	uvInfo.second.sideUV.y - 1,		0.0, 0.0, -1.0,
		   0.5f, -0.5f,  0.5f, uvInfo.second.sideUV.x,		uvInfo.second.sideUV.y - 1,		0.0, 0.0, -1.0,
		  -0.5f,  0.5f,  0.5f, uvInfo.second.sideUV.x - 1,	uvInfo.second.sideUV.y,			0.0, 0.0, -1.0,
		   0.5f,  0.5f,  0.5f, uvInfo.second.sideUV.x,		uvInfo.second.sideUV.y,			0.0, 0.0, -1.0,
		   // Face 3
		   0.5f, -0.5f,  0.5f, uvInfo.second.sideUV.x - 1,	uvInfo.second.sideUV.y - 1,		1.0, 0.0, 0.0,
		   0.5f, -0.5f, -0.5f, uvInfo.second.sideUV.x,		uvInfo.second.sideUV.y - 1,		1.0, 0.0, 0.0,
		   0.5f,  0.5f,  0.5f, uvInfo.second.sideUV.x - 1,	uvInfo.second.sideUV.y,			1.0, 0.0, 0.0,
		   0.5f,  0.5f, -0.5f, uvInfo.second.sideUV.x,		uvInfo.second.sideUV.y,			1.0, 0.0, 0.0,
		   // Face 4
		   0.5f, -0.5f, -0.5f, uvInfo.second.sideUV.x - 1,	uvInfo.second.sideUV.y - 1,		0.0, 0.0, 1.0,
		  -0.5f, -0.5f, -0.5f, uvInfo.second.sideUV.x,		uvInfo.second.sideUV.y - 1,		0.0, 0.0, 1.0,
		   0.5f,  0.5f, -0.5f, uvInfo.second.sideUV.x - 1,	uvInfo.second.sideUV.y,			0.0, 0.0, 1.0,
		  -0.5f,  0.5f, -0.5f, uvInfo.second.sideUV.x,		uvInfo.second.sideUV.y,			0.0, 0.0, 1.0,
		  // Top
		 -0.5f,  0.5f,  0.5f, uvInfo.second.topUV.x - 1,	uvInfo.second.topUV.y - 1,		0.0, 1.0, 0.0,
		 -0.5f,  0.5f, -0.5f, uvInfo.second.topUV.x,		uvInfo.second.topUV.y - 1,		0.0, 1.0, 0.0,
		  0.5f,  0.5f,  0.5f, uvInfo.second.topUV.x - 1,	uvInfo.second.topUV.y,			0.0, 1.0, 0.0,
		  0.5f,  0.5f, -0.5f, uvInfo.second.topUV.x,		uvInfo.second.topUV.y,			0.0, 1.0, 0.0,
		  // Bottom
		 -0.5f, -0.5f,  0.5f, uvInfo.second.bottomUV.x - 1,	uvInfo.second.bottomUV.y - 1,	0.0, -1.0, 0.0,
		 -0.5f, -0.5f, -0.5f, uvInfo.second.bottomUV.x,		uvInfo.second.bottomUV.y - 1,	0.0, -1.0, 0.0,
		  0.5f, -0.5f,  0.5f, uvInfo.second.bottomUV.x - 1,	uvInfo.second.bottomUV.y,		0.0, -1.0, 0.0,
		  0.5f, -0.5f, -0.5f, uvInfo.second.bottomUV.x,		uvInfo.second.bottomUV.y,		0.0, -1.0, 0.0,
		};

		vaMap[uvInfo.first] = vertices;
	}
}

BlockAtlas::~BlockAtlas() {
}

std::vector<float> BlockAtlas::GetBlockVertexArray(BlockAtlas::Type type) {
	return vaMap[type];
}

BlockAtlas::BlockUV BlockAtlas::GetBlockUVs(Type blockType)
{
	return uvMap[blockType];
}

