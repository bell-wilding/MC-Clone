#include "CollisionDetection.h"

#include <iostream>
#include "Print.h"

#include <algorithm>

bool CollisionDetection::RayAABBIntersection(const Ray& r, glm::vec3 boxPos, glm::vec3 boxSize, Ray::Collision& collision) {

	glm::vec3 boxMin = boxPos - boxSize;
	glm::vec3 boxMax = boxPos + boxSize;

	glm::vec3 rayPos = r.GetPosition();
	glm::vec3 rayDir = r.GetDirection();

	glm::vec3 tVals(-1, -1, -1);

	for (int i = 0; i < 3; ++i) {
		if (rayDir[i] > 0) {
			tVals[i] = (boxMin[i] - rayPos[i]) / rayDir[i];
		}
		else if (rayDir[i] < 0) {
			tVals[i] = (boxMax[i] - rayPos[i]) / rayDir[i];
		}
	}
	float bestT = std::max(std::max(tVals.x, tVals.y), tVals.z);
	if (bestT < 0.0f) {
		return false;
	}

	glm::vec3 intersection = rayPos + (rayDir * bestT);
	const float epsilon = 0.0001f;
	for (int i = 0; i < 3; ++i) {
		if (intersection[i] + epsilon < boxMin[i] || intersection[i] - epsilon > boxMax[i]) {
			return false;
		}
	}
	collision.collidedAt = intersection;
	collision.rayDistance = bestT;
	collision.normal = GetCollisionNormal(boxPos, intersection);

	return true;
}

bool CollisionDetection::AABBTest(const glm::vec3& posA, const glm::vec3& posB, const glm::vec3& halfSizeA, const glm::vec3& halfSizeB) {

	glm::vec3 delta = posB - posA;
	glm::vec3 totalSize = halfSizeA + halfSizeB;

	if (abs(delta.x) < totalSize.x &&
		abs(delta.y) < totalSize.y &&
		abs(delta.z) < totalSize.z) {
		return true;
	}
	return false;
}

bool CollisionDetection::AABBIntersection(const AABB& boxA, const glm::vec3& positionA, const AABB& boxB, const glm::vec3& positionB, ContactPoint& collisionInfo) {
	glm::vec3 boxAPos = positionA + boxA.GetOffset();
	glm::vec3 boxBPos = positionB + boxB.GetOffset();

	glm::vec3 boxASize = boxA.GetHalfDimensions();
	glm::vec3 boxBSize = boxB.GetHalfDimensions();

	bool overlap = AABBTest(boxAPos, boxBPos, boxASize, boxBSize);

	if (overlap) {
		static const glm::vec3 faces[6] = {
			glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0),
			glm::vec3(0, -1, 0), glm::vec3(0, 1, 0),
			glm::vec3(0, 0, -1), glm::vec3(0, 0, 1)
		};

		glm::vec3 maxA = boxAPos + boxASize;
		glm::vec3 minA = boxAPos - boxASize;

		glm::vec3 maxB = boxBPos + boxBSize;
		glm::vec3 minB = boxBPos - boxBSize;

		float distances[6] = {
			(maxB.x - minA.x),
			(maxA.x - minB.x),
			(maxB.y - minA.y),
			(maxA.y - minB.y),
			(maxB.z - minA.z),
			(maxA.z - minB.z)
		};
		float penetration = FLT_MAX;
		glm::vec3 bestAxis;

		for (int i = 0; i < 6; i++) {
			if (distances[i] < penetration) {
				penetration = distances[i];
				bestAxis = faces[i];
			}
		}
		collisionInfo.Set(glm::vec3(0), glm::vec3(0), bestAxis, penetration);
		return true;
	}
	return false;
}

glm::ivec3 CollisionDetection::GetCollisionNormal(glm::vec3 position, glm::vec3 intersection) {
	if (position.x + 0.5f == intersection.x) {
		return glm::ivec3(1, 0, 0);
	}
	if (position.x - 0.5f == intersection.x) {
		return glm::ivec3(-1, 0, 0);
	}
	if (position.y + 0.5f == intersection.y) {
		return glm::ivec3(0, 1, 0);
	}
	if (position.y - 0.5f == intersection.y) {
		return glm::ivec3(0, -1, 0);
	}
	if (position.z + 0.5f == intersection.z) {
		return glm::ivec3(0, 0, 1);
	}
	if (position.z - 0.5f == intersection.z) {
		return glm::ivec3(0, 0, -1);
	}
	return glm::ivec3(0, 0, 0);
}
