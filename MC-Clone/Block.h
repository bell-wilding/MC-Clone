#pragma once

#include "Object.h"
#include "VertexArray.h"

class Block : public Object {
public:
	enum Type {
		GRASS,
		STONE,
		DIRT,
		WOOD_PLANK,
		WOOD_LOG,
		SAND,
		COBBLESTONE
	};

	Block(Block::Type type, glm::vec3 position, glm::vec2 topUV, glm::vec2 bottomUV, glm::vec2 sideUV);
	Block(const Block &b);

	~Block();

	VertexArray GetVertexArray() const { return va; }

protected:
	VertexArray va;
	VertexBuffer vb;
	VertexBufferLayout layout;

	Block::Type type;

};

