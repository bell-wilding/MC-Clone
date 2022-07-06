#pragma once

#include "Ray.h"
#include <glm/glm.hpp>

class CollisionDetection {
public:
	static bool RayAABBIntersection(const Ray& r, glm::vec3 boxPos, glm::vec3 boxSize, Ray::Collision& collision);

protected:
	static glm::ivec3 GetCollisionNormal(glm::vec3 position, glm::vec3 intersection);
};