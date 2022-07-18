#pragma once

#include "Ray.h"
#include "AABB.h"
#include <glm/glm.hpp>

class CollisionDetection {
public:
	struct ContactPoint {
		void Set(glm::vec3 localA, glm::vec3 localB, glm::vec3 normal, float penetration) {
			this->localA = localA;
			this->localB = localB;
			this->normal = normal;
			this->penetration = penetration;
		}
		glm::vec3 localA;
		glm::vec3 localB;
		glm::vec3 normal;
		float penetration;
	};

	static bool RayAABBIntersection(const Ray& r, glm::vec3 boxPos, glm::vec3 boxSize, Ray::Collision& collision);

	static bool AABBIntersection(const AABB& boxA, const glm::vec3& positionA,
		const AABB& boxB, const glm::vec3& positionB, ContactPoint& collisionInfo);

protected:
	static glm::ivec3 GetCollisionNormal(glm::vec3 position, glm::vec3 intersection);

	static bool	AABBTest(const glm::vec3& posA, const glm::vec3& posB, const glm::vec3& halfSizeA, const glm::vec3& halfSizeB);
};