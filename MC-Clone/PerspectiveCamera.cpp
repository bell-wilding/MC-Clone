#include "PerspectiveCamera.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera(Input* input, const glm::vec3& pos, float fov, float nPlane, float fPlane, float pitch, float yaw) {
	this->input			= input;
	this->position		= pos;
	this->fieldOfView	= fov;
	this->nearPlane		= nPlane;
	this->farPlane		= fPlane;
	this->pitch			= pitch;
	this->yaw			= yaw;

	prevCursorPos = input->GetCursorPosition();

	increaseSpeed = false;
	moveSpeed = 10.0f;
	turnSensitivity = 0.1f;

	controlsActive = true;
}

PerspectiveCamera::~PerspectiveCamera() {
}

void PerspectiveCamera::Update(float dt) {

	glm::vec2 currentCursorPos = input->GetCursorPosition();

	if (controlsActive) {

		glm::vec2 relativePos(currentCursorPos.x - prevCursorPos.x, currentCursorPos.y - prevCursorPos.y);

		pitch -= relativePos.y * turnSensitivity;
		yaw -= relativePos.x * turnSensitivity;

		prevCursorPos = currentCursorPos;

		pitch = std::min(pitch, 90.0f);
		pitch = std::max(pitch, -90.0f);

		if (yaw < 0)
			yaw += 360.0f;

		if (yaw > 360.0f)
			yaw -= 360.0f;
	}
	else {
		prevCursorPos = currentCursorPos;
	}
}

glm::mat4 PerspectiveCamera::BuildViewMatrix() {
	viewMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-pitch), glm::vec3(1, 0, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(-yaw), glm::vec3(0, 1, 0))
		* glm::translate(glm::mat4(1.0f), -position);
	return viewMatrix;
}

glm::mat4 PerspectiveCamera::BuildProjectionMatrix() {
	glm::vec2 windowDimensions = input->GetWindowSize();
	float aspect = (float)windowDimensions.x / (float)windowDimensions.y;
	projectionMatrix = glm::perspective(fieldOfView, aspect, nearPlane, farPlane);
	return projectionMatrix;
}

glm::vec3 PerspectiveCamera::ForwardVector() {
	return glm::normalize(glm::vec3(-viewMatrix[0][2], -viewMatrix[1][2], -viewMatrix[2][2]));
}

glm::vec3 PerspectiveCamera::RightVector() {
	return glm::normalize(glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]));
}

glm::vec3 PerspectiveCamera::UpVector() {
	return glm::normalize(glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]));
}
