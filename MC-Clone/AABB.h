#pragma once

#include <glm/glm.hpp>

class AABB {
public:
	AABB(const glm::vec3& halfDimensions = glm::vec3(0.5f, 0.5f, 0.5f), const glm::vec3& offset = glm::vec3(0, 0, 0)) : halfDimensions(halfDimensions), offset(offset) {};
	~AABB() {}

	glm::vec3 GetHalfDimensions() const {
		return halfDimensions;
	}

	glm::vec3 GetOffset() const {
		return offset;
	}

protected:
	glm::vec3 halfDimensions;
	glm::vec3 offset;
};