#pragma once

#include <glm/glm.hpp>

class DirectionalLight {
public:
	DirectionalLight(glm::vec3 direction, float intensity) : direction(glm::normalize(direction)), intensity(intensity) {};
	~DirectionalLight() {};

	glm::vec3 GetLightDirection() const { return direction; }
	float GetLightIntensity() const { return intensity; }

	void SetDirection(const glm::vec3 dir) { direction = glm::normalize(dir); }
	void SetIntensity(float i) { intensity = i; }

	void RotateDirection(const glm::vec3 axis, float theta) { direction = direction* glm::cos(theta) + glm::cross(axis, direction) * glm::sin(theta) + axis * glm::dot(axis, direction) * (1 - glm::cos(theta)); }

protected:

	glm::vec3 direction;
	float intensity;
};