#pragma once

#include "BlockAtlas.h"
#include "Object.h"

class Block {
public:
	Block(BlockAtlas::Type type) : type(type) {};
	~Block() {};

	BlockAtlas::Type GetType() const { return type; }

protected:
	BlockAtlas::Type type;

};