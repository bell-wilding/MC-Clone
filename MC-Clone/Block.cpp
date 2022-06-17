#include "Block.h"
#include "IndexBuffer.h"

Block::Block(Block::Type type, glm::vec3 position, glm::vec2 topUV, glm::vec2 bottomUV, glm::vec2 sideUV) : Object(position, glm::vec3(1), glm::vec3(0)) {

	//this->type = type;

	//float vertices[] = {
	//	// Face 1
	//  -0.5f, -0.5f, -0.5f, sideUV.x - 1, sideUV.y - 1,
	//  -0.5f, -0.5f,  0.5f, sideUV.x,	 sideUV.y - 1,
	//  -0.5f,  0.5f, -0.5f, sideUV.x - 1, sideUV.y,
	//  -0.5f,  0.5f,  0.5f, sideUV.x,	 sideUV.y,
	//  // Face 2
	//  -0.5f, -0.5f,  0.5f, sideUV.x - 1, sideUV.y - 1,
	//   0.5f, -0.5f,  0.5f, sideUV.x,	 sideUV.y - 1,
	//  -0.5f,  0.5f,  0.5f, sideUV.x - 1, sideUV.y,
	//   0.5f,  0.5f,  0.5f, sideUV.x,	 sideUV.y,
	//   // Face 3
	//   0.5f, -0.5f,  0.5f, sideUV.x - 1, sideUV.y - 1,
	//   0.5f, -0.5f, -0.5f, sideUV.x,	 sideUV.y - 1,
	//   0.5f,  0.5f,  0.5f, sideUV.x - 1, sideUV.y,
	//   0.5f,  0.5f, -0.5f, sideUV.x,	 sideUV.y,
	//   // Face 4
	//   0.5f, -0.5f, -0.5f, sideUV.x - 1, sideUV.y - 1,
	//  -0.5f, -0.5f, -0.5f, sideUV.x,	 sideUV.y - 1,
	//   0.5f,  0.5f, -0.5f, sideUV.x - 1, sideUV.y,
	//  -0.5f,  0.5f, -0.5f, sideUV.x,	 sideUV.y,
	//  // Top
	// -0.5f,  0.5f,  0.5f, topUV.x - 1,	topUV.y - 1,
	// -0.5f,  0.5f, -0.5f, topUV.x,		topUV.y - 1,
	//  0.5f,  0.5f,  0.5f, topUV.x - 1,	topUV.y,
	//  0.5f,  0.5f, -0.5f, topUV.x,		topUV.y,
	//  // Bottom
	// -0.5f, -0.5f,  0.5f, bottomUV.x - 1,	bottomUV.y - 1,
	// -0.5f, -0.5f, -0.5f, bottomUV.x,		bottomUV.y - 1,
	//  0.5f, -0.5f,  0.5f, bottomUV.x - 1,	bottomUV.y,
	//  0.5f, -0.5f, -0.5f, bottomUV.x,		bottomUV.y,
	//};

	//vb = VertexBuffer(vertices, 6 * 4 * 5 * sizeof(float));
	//layout.Push<float>(3);
	//layout.Push<float>(2);
	//va.AddBuffer(vb, layout);
}

Block::Block(const Block & b) : Object(position, glm::vec3(1), glm::vec3(0)) {
	type = b.type;
	va = b.va;
	layout = b.layout;
	vb = b.vb;
}

Block::~Block() {
}

