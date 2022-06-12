#include "PerspectiveCamera.h"
#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera(GLFWwindow* window, const glm::vec3& pos, float fov, float nPlane, float fPlane, float pitch, float yaw) {
	this->window		= window;
	this->position		= pos;
	this->fieldOfView	= fov;
	this->nearPlane		= nPlane;
	this->farPlane		= fPlane;
	this->pitch			= pitch;
	this->yaw			= yaw;

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	this->prevX = x;
	this->prevY	= y;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

PerspectiveCamera::~PerspectiveCamera() {
}

void PerspectiveCamera::Update() {
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	glm::vec2 relativePos(x - prevX, y - prevY);

	float turnSpeed = 0.1f;

	pitch	-= relativePos.y * turnSpeed;
	yaw		-= relativePos.x * turnSpeed;

	prevX = x;
	prevY = y;

	pitch = std::min(pitch,  90.0f);
	pitch = std::max(pitch, -90.0f);

	if (yaw < 0)
		yaw += 360.0f;
	
	if (yaw > 360.0f)
		yaw -= 360.0f;

	float frameSpeed = 0.1f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		glm::mat4 posM = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0, 1, 0));
		glm::vec3 posDif = (posM * glm::vec4(0, 0, -1, 1) * frameSpeed);
		position += posDif;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		glm::mat4 posM = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0, 1, 0));
		glm::vec3 posDif = (posM * glm::vec4(0, 0, -1, 1) * frameSpeed);
		position -= posDif;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		glm::mat4 posM = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0, 1, 0));
		glm::vec3 posDif = (posM * glm::vec4(-1, 0, 0, 1) * frameSpeed);
		position += posDif;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		glm::mat4 posM = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0, 1, 0));
		glm::vec3 posDif = (posM * glm::vec4(-1, 0, 0, 1) * frameSpeed);
		position -= posDif;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		position.y += frameSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		position.y -= frameSpeed;
	}
}

glm::mat4 PerspectiveCamera::BuildViewMatrix() const {
	return glm::rotate(glm::mat4(1.0f), glm::radians(-pitch), glm::vec3(1, 0, 0)) 
		* glm::rotate(glm::mat4(1.0f), glm::radians(-yaw), glm::vec3(0, 1, 0))
		* glm::translate(glm::mat4(1.0f), -position);
}

glm::mat4 PerspectiveCamera::BuildProjectionMatrix() const {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	float aspect = (float)width / (float)height;

	return glm::perspective(fieldOfView, aspect, nearPlane, farPlane);
}
