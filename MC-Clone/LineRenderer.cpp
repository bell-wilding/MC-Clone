
#include <GL/glew.h>

#include "LineRenderer.h"

LineRenderer::LineRenderer(PerspectiveCamera* cam) : shader("res/shaders/Line.shader"), camera(cam) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

LineRenderer::~LineRenderer() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void LineRenderer::DrawLine(glm::vec3 startPos, glm::vec3 endPos, glm::vec4 colour) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float vertices[6] = {
		startPos.x, startPos.y, startPos.z,
		endPos.x, endPos.y, endPos.z
	};

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW);

	glm::mat4 vpMatrix = camera->BuildProjectionMatrix() * camera->BuildViewMatrix();

	shader.Bind();
	shader.SetUniform4f("u_Colour", colour.r, colour.g, colour.b, colour.a);
	shader.SetUniformMat4f("u_VP", vpMatrix);

	glDrawArrays(GL_LINES, 0, 2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
