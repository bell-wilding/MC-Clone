#pragma once

#include <map>
#include <glm/glm.hpp>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "PerspectiveCamera.h"

class BlockRenderer {
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

	struct BlockUV {
		glm::vec2 topUV;
		glm::vec2 bottomUV;
		glm::vec2 sideUV;
	};

	BlockRenderer();
	~BlockRenderer();

	void Clear();
	void RenderBlock(BlockRenderer::Type type, glm::mat4 blockWorldMatrix, PerspectiveCamera &cam);

protected:

	std::map<Type, BlockUV> uvMap;
	VertexArray va;
	VertexBuffer vb;
	VertexBufferLayout layout;
	IndexBuffer ib;
	Shader shader;
	Texture texture;

};

