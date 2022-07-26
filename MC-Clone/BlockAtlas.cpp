#include "BlockAtlas.h"

BlockAtlas::BlockAtlas() {

	uvMap[BlockAtlas::Type::AIR]			= { glm::vec2(14, 10), glm::vec2(14, 10), glm::vec2(14, 10) };
	uvMap[BlockAtlas::Type::GRASS]			= { glm::vec2(1, 16), glm::vec2(3, 16), glm::vec2(4, 16) };
	uvMap[BlockAtlas::Type::OAK_LOG]		= { glm::vec2(6, 15), glm::vec2(6, 15), glm::vec2(5, 15) };
	uvMap[BlockAtlas::Type::BIRCH_LOG]		= { glm::vec2(6, 15), glm::vec2(6, 15), glm::vec2(6, 9) };
	uvMap[BlockAtlas::Type::COBBLESTONE]	= { glm::vec2(1, 15), glm::vec2(1, 15), glm::vec2(1, 15) };
	uvMap[BlockAtlas::Type::WOOD_PLANK]		= { glm::vec2(5, 16), glm::vec2(5, 16), glm::vec2(5, 16) };
	uvMap[BlockAtlas::Type::LEAF]			= { glm::vec2(5, 13), glm::vec2(5, 13), glm::vec2(5, 13) };
	uvMap[BlockAtlas::Type::STONE]			= { glm::vec2(2, 16), glm::vec2(2, 16), glm::vec2(2, 16) };
	uvMap[BlockAtlas::Type::SAND]			= { glm::vec2(3, 15), glm::vec2(3, 15), glm::vec2(3, 15) };
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
	uvMap[BlockAtlas::Type::COAL_ORE]		= { glm::vec2(3, 14), glm::vec2(3, 14), glm::vec2(3, 14) };
	uvMap[BlockAtlas::Type::IRON_ORE]		= { glm::vec2(2, 14), glm::vec2(2, 14), glm::vec2(2, 14) };
	uvMap[BlockAtlas::Type::GOLD_ORE]		= { glm::vec2(1, 14), glm::vec2(1, 14), glm::vec2(1, 14) };
	uvMap[BlockAtlas::Type::DIAMOND_ORE]	= { glm::vec2(3, 13), glm::vec2(3, 13), glm::vec2(3, 13) };
	uvMap[BlockAtlas::Type::BREAK_1]		= { glm::vec2(1, 1), glm::vec2(1, 1), glm::vec2(1, 1) };
	uvMap[BlockAtlas::Type::BREAK_2]		= { glm::vec2(2, 1), glm::vec2(2, 1), glm::vec2(2, 1) };
	uvMap[BlockAtlas::Type::BREAK_3]		= { glm::vec2(3, 1), glm::vec2(3, 1), glm::vec2(3, 1) };
	uvMap[BlockAtlas::Type::BREAK_4]		= { glm::vec2(4, 1), glm::vec2(4, 1), glm::vec2(4, 1) };
	uvMap[BlockAtlas::Type::BREAK_5]		= { glm::vec2(5, 1), glm::vec2(5, 1), glm::vec2(5, 1) };
	uvMap[BlockAtlas::Type::BREAK_6]		= { glm::vec2(6, 1), glm::vec2(6, 1), glm::vec2(6, 1) };
	uvMap[BlockAtlas::Type::BREAK_7]		= { glm::vec2(7, 1), glm::vec2(7, 1), glm::vec2(7, 1) };
	uvMap[BlockAtlas::Type::BREAK_8]		= { glm::vec2(8, 1), glm::vec2(8, 1), glm::vec2(8, 1) };
	uvMap[BlockAtlas::Type::BREAK_9]		= { glm::vec2(9, 1), glm::vec2(9, 1), glm::vec2(9, 1) };
	uvMap[BlockAtlas::Type::BREAK_10]		= { glm::vec2(10, 1), glm::vec2(10, 1), glm::vec2(10, 1) };
	uvMap[BlockAtlas::Type::SNOW]			= { glm::vec2(3, 12), glm::vec2(3, 16), glm::vec2(5, 12) };

	nameMap[BlockAtlas::Type::AIR]				= "Air";
	nameMap[BlockAtlas::Type::GRASS]			= "Grass";
	nameMap[BlockAtlas::Type::STONE]			= "Stone";
	nameMap[BlockAtlas::Type::DIRT]				= "Dirt";
	nameMap[BlockAtlas::Type::WOOD_PLANK]		= "Wood Plank";
	nameMap[BlockAtlas::Type::OAK_LOG]			= "Oak Log";
	nameMap[BlockAtlas::Type::BIRCH_LOG]		= "Birch Log";
	nameMap[BlockAtlas::Type::LEAF]				= "Leaf";
	nameMap[BlockAtlas::Type::SAND]				= "Sand";
	nameMap[BlockAtlas::Type::COBBLESTONE]		= "Cobblestone";
	nameMap[BlockAtlas::Type::BEDROCK]			= "Bedrock";
	nameMap[BlockAtlas::Type::WINDOW]			= "Window";
	nameMap[BlockAtlas::Type::TALL_GRASS]		= "Tall Grass";
	nameMap[BlockAtlas::Type::RED_FLOWER]		= "Red Glower";
	nameMap[BlockAtlas::Type::YELLOW_FLOWER]	= "Yellow Flower";
	nameMap[BlockAtlas::Type::SAPLING]			= "Sapling";
	nameMap[BlockAtlas::Type::RED_MUSHROOM]		= "Red Mushroom";
	nameMap[BlockAtlas::Type::BROWN_MUSHROOM]	= "Brown Mushroom";
	nameMap[BlockAtlas::Type::SAD_COWBOY]		= "Sad Cowboy";
	nameMap[BlockAtlas::Type::WATER_TOP]		= "Water Top";
	nameMap[BlockAtlas::Type::WATER]			= "Water";
	nameMap[BlockAtlas::Type::COAL_ORE]			= "Coal Ore";
	nameMap[BlockAtlas::Type::IRON_ORE]			= "Iron Ore";
	nameMap[BlockAtlas::Type::GOLD_ORE]			= "Gold Ore";
	nameMap[BlockAtlas::Type::DIAMOND_ORE]		= "Diamond Ore";
	nameMap[BlockAtlas::Type::BREAK_1]			= "Break 1";
	nameMap[BlockAtlas::Type::BREAK_2]			= "Break 2";
	nameMap[BlockAtlas::Type::BREAK_3]			= "Break 3";
	nameMap[BlockAtlas::Type::BREAK_4]			= "Break 4";
	nameMap[BlockAtlas::Type::BREAK_5]			= "Break 5";
	nameMap[BlockAtlas::Type::BREAK_6]			= "Break 6";
	nameMap[BlockAtlas::Type::BREAK_7]			= "Break 7";
	nameMap[BlockAtlas::Type::BREAK_8]			= "Break 8";
	nameMap[BlockAtlas::Type::BREAK_9]			= "Break 9";
	nameMap[BlockAtlas::Type::BREAK_10]			= "Break 10";
	nameMap[BlockAtlas::Type::SNOW]				= "Snow";

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

