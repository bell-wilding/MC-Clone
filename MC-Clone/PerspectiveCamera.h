#pragma once

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

class PerspectiveCamera {
public:
	PerspectiveCamera(GLFWwindow* window, const glm::vec3& pos, float fov, float nPlane, float fPlane, float pitch, float yaw);
	~PerspectiveCamera();

	void Update();

	glm::mat4 BuildViewMatrix() const;
	glm::mat4 BuildProjectionMatrix() const;

protected:

	glm::vec3 position;
	float pitch;
	float yaw;

	float nearPlane;
	float farPlane;

	float fieldOfView;

	GLFWwindow* window;

	double prevX, prevY;
};