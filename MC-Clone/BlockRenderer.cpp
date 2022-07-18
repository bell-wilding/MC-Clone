#include "BlockRenderer.h"

#include <gl/glew.h>

BlockRenderer::BlockRenderer() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(5 * sizeof(float)));

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
}

BlockRenderer::~BlockRenderer() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &vao);
}

void BlockRenderer::DrawBlock(BlockAtlas::Type type, glm::ivec3 position) {

	BlockAtlas b;
	BlockAtlas::BlockUV uvs = b.GetBlockUVs(type);

	float vertices[] = {
		// Face 1
	  position.x + -0.502f, position.y + -0.502f, position.z + -0.502f, uvs.sideUV.x - 1,	uvs.sideUV.y - 1,	1.0f, 0.0f, 0.0f,
	  position.x + -0.502f, position.y + -0.502f, position.z + 0.502f,  uvs.sideUV.x,		uvs.sideUV.y - 1,	1.0f, 0.0f, 0.0f,
	  position.x + -0.502f, position.y + 0.502f,  position.z + -0.502f, uvs.sideUV.x - 1,	uvs.sideUV.y,		1.0f, 0.0f, 0.0f,
	  position.x + -0.502f, position.y + 0.502f,  position.z + 0.502f,  uvs.sideUV.x,		uvs.sideUV.y,		1.0f, 0.0f, 0.0f,
	  // Face 2
	  position.x + -0.502f, position.y + -0.502f, position.z + 0.502f, uvs.sideUV.x - 1,	uvs.sideUV.y - 1,	0.0f, 0.0f, -1.0f,
	  position.x + 0.502f,  position.y + -0.502f, position.z + 0.502f, uvs.sideUV.x,		uvs.sideUV.y - 1,	0.0f, 0.0f, -1.0f,
	  position.x + -0.502f, position.y + 0.502f,  position.z + 0.502f, uvs.sideUV.x - 1,	uvs.sideUV.y,		0.0f, 0.0f, -1.0f,
	  position.x + 0.502f,  position.y + 0.502f,  position.z + 0.502f, uvs.sideUV.x,		uvs.sideUV.y,		0.0f, 0.0f, -1.0f,
	  // Face 3
	  position.x + 0.502f,  position.y + -0.502f, position.z + 0.502f,  uvs.sideUV.x - 1,	uvs.sideUV.y - 1,	-1.0f, 0.0f, 0.0f,
	  position.x + 0.502f,  position.y + -0.502f, position.z + -0.502f, uvs.sideUV.x,		uvs.sideUV.y - 1,	-1.0f, 0.0f, 0.0f,
	  position.x + 0.502f,  position.y + 0.502f,  position.z + 0.502f,  uvs.sideUV.x - 1,	uvs.sideUV.y,		-1.0f, 0.0f, 0.0f,
	  position.x + 0.502f,  position.y + 0.502f,  position.z + -0.502f, uvs.sideUV.x,		uvs.sideUV.y,		-1.0f, 0.0f, 0.0f,
	  // Face 4
	  position.x + 0.502f,  position.y + -0.502f, position.z + -0.502f, uvs.sideUV.x - 1,	uvs.sideUV.y - 1,	0.0f, 0.0f, 1.0f,
	  position.x + -0.502f, position.y + -0.502f, position.z + -0.502f, uvs.sideUV.x,		uvs.sideUV.y - 1,	0.0f, 0.0f, 1.0f,
	  position.x + 0.502f,  position.y + 0.502f,  position.z + -0.502f, uvs.sideUV.x - 1,	uvs.sideUV.y,		0.0f, 0.0f, 1.0f,
	  position.x + -0.502f, position.y + 0.502f,  position.z + -0.502f, uvs.sideUV.x,		uvs.sideUV.y,		0.0f, 0.0f, 1.0f,
	  // Top
	 position.x + -0.502f,  position.y + 0.502f,  position.z + 0.502f,  uvs.topUV.x - 1,	uvs.topUV.y - 1,		0.0f, -1.0f, 0.0f,
	 position.x + -0.502f,  position.y + 0.502f,  position.z + -0.502f, uvs.topUV.x,		uvs.topUV.y - 1,		0.0f, -1.0f, 0.0f,
	 position.x + 0.502f,   position.y + 0.502f,  position.z + 0.502f,  uvs.topUV.x - 1,	uvs.topUV.y,			0.0f, -1.0f, 0.0f,
	 position.x + 0.502f,   position.y + 0.502f,  position.z + -0.502f, uvs.topUV.x,		uvs.topUV.y,			0.0f, -1.0f, 0.0f,
	  // Bottom
	 position.x + -0.502f,  position.y + -0.502f,  position.z + 0.502f,  uvs.bottomUV.x - 1,	uvs.bottomUV.y - 1, 0.0f, 1.0f, 0.0f,
	 position.x + -0.502f,  position.y + -0.502f,  position.z + -0.502f, uvs.bottomUV.x,		uvs.bottomUV.y - 1, 0.0f, 1.0f, 0.0f,
	 position.x + 0.502f,   position.y + -0.502f,  position.z + 0.502f,  uvs.bottomUV.x - 1,	uvs.bottomUV.y,		0.0f, 1.0f, 0.0f,
	 position.x + 0.502f,   position.y + -0.502f,  position.z + -0.502f, uvs.bottomUV.x,		uvs.bottomUV.y,		0.0f, 1.0f, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 2, 1, 3, 2,
		4, 5, 6, 5, 7, 6,
		8, 9, 10, 9, 11, 10,
		12, 13, 14, 13, 15, 14,
		18, 17, 16, 18, 19, 17,
		20, 21, 22, 21, 23, 22
	};

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 8 * 4 * 6 * sizeof(float), &vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 6 * sizeof(unsigned int), &indices, GL_STATIC_DRAW);

	glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, nullptr);
}
