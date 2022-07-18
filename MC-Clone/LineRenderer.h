#pragma once

#include <glm/glm.hpp>
#include "Shader.h"
#include "PerspectiveCamera.h"

class LineRenderer {
public:
	LineRenderer(PerspectiveCamera* cam);
	~LineRenderer();

	void DrawLine(glm::vec3 startPos, glm::vec3 endPos, glm::vec4 colour = glm::vec4(1, 1, 1, 1));

protected:
	unsigned int vao, vbo;

	Shader shader;

	PerspectiveCamera* camera;
};