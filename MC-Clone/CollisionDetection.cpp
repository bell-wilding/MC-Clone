#include "CollisionDetection.h"

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
