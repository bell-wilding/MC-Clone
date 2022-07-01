#include "BlockAtlas.h"

BlockAtlas::BlockAtlas() {

	uvMap[BlockAtlas::Type::GRASS]			= { glm::vec2(1, 16), glm::vec2(3, 16), glm::vec2(4, 16) };
	uvMap[BlockAtlas::Type::WOOD_LOG]		= { glm::vec2(6, 15), glm::vec2(6, 15), glm::vec2(5, 15) };
	uvMap[BlockAtlas::Type::COBBLESTONE]	= { glm::vec2(1, 15), glm::vec2(1, 15), glm::vec2(1, 15) };
	uvMap[BlockAtlas::Type::WOOD_PLANK]		= { glm::vec2(5, 16), glm::vec2(5, 16), glm::vec2(5, 16) };
	uvMap[BlockAtlas::Type::STONE]			= { glm::vec2(2, 16), glm::vec2(2, 16), glm::vec2(2, 16) };
	uvMap[BlockAtlas::Type::DIRT]			= { glm::vec2(3, 16), glm::vec2(3, 16), glm::vec2(3, 16) };

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

