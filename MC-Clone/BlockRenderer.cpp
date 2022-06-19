#include "BlockRenderer.h"

BlockRenderer::BlockRenderer() : shader("res/shaders/Basic.shader"), texture("res/textures/default_texture.png") {

	uvMap[BlockRenderer::Type::GRASS] = { glm::vec2(1, 16), glm::vec2(3, 16), glm::vec2(4, 16) };

	float vertices[] = {
		// Face 1
	  -0.5f, -0.5f, -0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x - 1,	uvMap[BlockRenderer::Type::GRASS].sideUV.y - 1,
	  -0.5f, -0.5f,  0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x,		uvMap[BlockRenderer::Type::GRASS].sideUV.y - 1,
	  -0.5f,  0.5f, -0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x - 1,	uvMap[BlockRenderer::Type::GRASS].sideUV.y,
	  -0.5f,  0.5f,  0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x,		uvMap[BlockRenderer::Type::GRASS].sideUV.y,
	  // Face 2
	  -0.5f, -0.5f,  0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x - 1,	uvMap[BlockRenderer::Type::GRASS].sideUV.y - 1,
	   0.5f, -0.5f,  0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x,		uvMap[BlockRenderer::Type::GRASS].sideUV.y - 1,
	  -0.5f,  0.5f,  0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x - 1,	uvMap[BlockRenderer::Type::GRASS].sideUV.y,
	   0.5f,  0.5f,  0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x,		uvMap[BlockRenderer::Type::GRASS].sideUV.y,
	   // Face 3
	   0.5f, -0.5f,  0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x - 1,	uvMap[BlockRenderer::Type::GRASS].sideUV.y - 1,
	   0.5f, -0.5f, -0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x,		uvMap[BlockRenderer::Type::GRASS].sideUV.y - 1,
	   0.5f,  0.5f,  0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x - 1,	uvMap[BlockRenderer::Type::GRASS].sideUV.y,
	   0.5f,  0.5f, -0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x,		uvMap[BlockRenderer::Type::GRASS].sideUV.y,
	   // Face 4
	   0.5f, -0.5f, -0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x - 1,	uvMap[BlockRenderer::Type::GRASS].sideUV.y - 1,
	  -0.5f, -0.5f, -0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x,		uvMap[BlockRenderer::Type::GRASS].sideUV.y - 1,
	   0.5f,  0.5f, -0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x - 1,	uvMap[BlockRenderer::Type::GRASS].sideUV.y,
	  -0.5f,  0.5f, -0.5f, uvMap[BlockRenderer::Type::GRASS].sideUV.x,		uvMap[BlockRenderer::Type::GRASS].sideUV.y,
	  // Top
	 -0.5f,  0.5f,  0.5f, uvMap[BlockRenderer::Type::GRASS].topUV.x - 1,	uvMap[BlockRenderer::Type::GRASS].topUV.y - 1,
	 -0.5f,  0.5f, -0.5f, uvMap[BlockRenderer::Type::GRASS].topUV.x,		uvMap[BlockRenderer::Type::GRASS].topUV.y - 1,
	  0.5f,  0.5f,  0.5f, uvMap[BlockRenderer::Type::GRASS].topUV.x - 1,	uvMap[BlockRenderer::Type::GRASS].topUV.y,
	  0.5f,  0.5f, -0.5f, uvMap[BlockRenderer::Type::GRASS].topUV.x,		uvMap[BlockRenderer::Type::GRASS].topUV.y,
	  // Bottom
	 -0.5f, -0.5f,  0.5f, uvMap[BlockRenderer::Type::GRASS].bottomUV.x - 1,	uvMap[BlockRenderer::Type::GRASS].bottomUV.y - 1,
	 -0.5f, -0.5f, -0.5f, uvMap[BlockRenderer::Type::GRASS].bottomUV.x,		uvMap[BlockRenderer::Type::GRASS].bottomUV.y - 1,
	  0.5f, -0.5f,  0.5f, uvMap[BlockRenderer::Type::GRASS].bottomUV.x - 1,	uvMap[BlockRenderer::Type::GRASS].bottomUV.y,
	  0.5f, -0.5f, -0.5f, uvMap[BlockRenderer::Type::GRASS].bottomUV.x,		uvMap[BlockRenderer::Type::GRASS].bottomUV.y
	};

	unsigned int indices[] = {
		0, 1, 2, 2, 3, 1,
		4, 5, 6, 6, 7, 5,
		8, 9, 10, 10, 11, 9,
		12, 13, 14, 14, 15, 13,
		16, 17, 18, 18, 19, 17,
		20, 21, 22, 22, 23, 21
	};

	vb = VertexBuffer(vertices, 6 * 4 * 5 * sizeof(float));
	layout.Push<float>(3);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	ib = IndexBuffer(indices, 6 * 6);

	shader.Bind();
	texture.Bind();

	shader.SetUniform1i("u_Texture", 0);
}

BlockRenderer::~BlockRenderer() {
}

void BlockRenderer::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void BlockRenderer::RenderBlock(BlockRenderer::Type type, glm::mat4 blockWorldMatrix, PerspectiveCamera& cam) {

	float vertices[] = {
		// Face 1
	  -0.5f, -0.5f, -0.5f, uvMap[type].sideUV.x - 1,	uvMap[type].sideUV.y - 1,
	  -0.5f, -0.5f,  0.5f, uvMap[type].sideUV.x,		uvMap[type].sideUV.y - 1,
	  -0.5f,  0.5f, -0.5f, uvMap[type].sideUV.x - 1,	uvMap[type].sideUV.y,
	  -0.5f,  0.5f,  0.5f, uvMap[type].sideUV.x,		uvMap[type].sideUV.y,
	  // Face 2
	  -0.5f, -0.5f,  0.5f, uvMap[type].sideUV.x - 1,	uvMap[type].sideUV.y - 1,
	   0.5f, -0.5f,  0.5f, uvMap[type].sideUV.x,		uvMap[type].sideUV.y - 1,
	  -0.5f,  0.5f,  0.5f, uvMap[type].sideUV.x - 1,	uvMap[type].sideUV.y,
	   0.5f,  0.5f,  0.5f, uvMap[type].sideUV.x,		uvMap[type].sideUV.y,
	   // Face 3
	   0.5f, -0.5f,  0.5f, uvMap[type].sideUV.x - 1,	uvMap[type].sideUV.y - 1,
	   0.5f, -0.5f, -0.5f, uvMap[type].sideUV.x,		uvMap[type].sideUV.y - 1,
	   0.5f,  0.5f,  0.5f, uvMap[type].sideUV.x - 1,	uvMap[type].sideUV.y,
	   0.5f,  0.5f, -0.5f, uvMap[type].sideUV.x,		uvMap[type].sideUV.y,
	   // Face 4
	   0.5f, -0.5f, -0.5f, uvMap[type].sideUV.x - 1,	uvMap[type].sideUV.y - 1,
	  -0.5f, -0.5f, -0.5f, uvMap[type].sideUV.x,		uvMap[type].sideUV.y - 1,
	   0.5f,  0.5f, -0.5f, uvMap[type].sideUV.x - 1,	uvMap[type].sideUV.y,
	  -0.5f,  0.5f, -0.5f, uvMap[type].sideUV.x,		uvMap[type].sideUV.y,
	  // Top
	 -0.5f,  0.5f,  0.5f, uvMap[type].topUV.x - 1,		uvMap[type].topUV.y - 1,
	 -0.5f,  0.5f, -0.5f, uvMap[type].topUV.x,			uvMap[type].topUV.y - 1,
	  0.5f,  0.5f,  0.5f, uvMap[type].topUV.x - 1,		uvMap[type].topUV.y,
	  0.5f,  0.5f, -0.5f, uvMap[type].topUV.x,			uvMap[type].topUV.y,
	  // Bottom
	 -0.5f, -0.5f,  0.5f, uvMap[type].bottomUV.x - 1,	uvMap[type].bottomUV.y - 1,
	 -0.5f, -0.5f, -0.5f, uvMap[type].bottomUV.x,		uvMap[type].bottomUV.y - 1,
	  0.5f, -0.5f,  0.5f, uvMap[type].bottomUV.x - 1,	uvMap[type].bottomUV.y,
	  0.5f, -0.5f, -0.5f, uvMap[type].bottomUV.x,		uvMap[type].bottomUV.y
	};

	shader.Bind();
	glm::mat4 mvp = cam.BuildProjectionMatrix() * cam.BuildViewMatrix() * blockWorldMatrix;
	shader.SetUniformMat4f("u_MVP", mvp);

	va.Bind();
	vb.BufferData(vertices, 6 * 4 * 5 * sizeof(float));
	ib.Bind();
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

