#pragma once

#include <glm/glm.hpp>

class Ray {
public:
	struct Collision {
		glm::vec3 collidedAt;
		glm::ivec3 normal;
		float rayDistance;
	};

	Ray(glm::vec3 position, glm::vec3 direction) {
		this->position = position;
		this->direction = glm::normalize(direction);
	}
	~Ray(void) {}

	glm::vec3 GetPosition() const { return position; }

	glm::vec3 GetDirection() const { return direction; }

protected:
	glm::vec3 position;	
	glm::vec3 direction;	
};