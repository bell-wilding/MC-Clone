#pragma once

#include <glm/glm.hpp>

class Object {
public:
	Object(glm::vec3 position, glm::vec3 scale, glm::vec3 orientation);
	~Object();

	inline glm::mat4 GetWorldMatrix() const { return worldMatrix; }

protected:
	glm::mat4 ToMatrix(const glm::vec4& orientation);
	glm::vec4 ToQuaternion(const glm::vec3& eulerOrientation);

	glm::mat4 worldMatrix;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 orientation;
};