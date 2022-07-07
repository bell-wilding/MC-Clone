#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;

class PerspectiveCamera {
public:
	PerspectiveCamera(GLFWwindow* window, const glm::vec3& pos, float fov, float nPlane, float fPlane, float pitch, float yaw);
	~PerspectiveCamera();

	void Update(float dt);

	glm::mat4 BuildViewMatrix();
	glm::mat4 BuildProjectionMatrix();

	glm::vec3 ForwardVector();

	glm::vec3 GetPosition() const { return position; }

	void SetPosition(const glm::vec3 pos) { position = pos; }

	float GetYaw() const { return yaw; }
	float GetPitch() const { return pitch; }

	void SetControlsActive(bool active) { controlsActive = active; }

protected:

	glm::vec3 position;
	float pitch;
	float yaw;

	float nearPlane;
	float farPlane;

	float fieldOfView;

	GLFWwindow* window;

	double prevX, prevY;

	float moveSpeed;
	float turnSensitivity;

	bool increaseSpeed;

	bool controlsActive;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
};