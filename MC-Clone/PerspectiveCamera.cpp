#include "PerspectiveCamera.h"
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

PerspectiveCamera::~PerspectiveCamera() {
}

void PerspectiveCamera::Update() {
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	glm::vec2 relativePos(x - prevX, y - prevY);

	pitch	-= relativePos.y;
	yaw		-= relativePos.x;

	pitch = std::min(pitch,  90.0f);
	pitch = std::max(pitch, -90.0f);

	if (yaw < 0)
		yaw += 360.0f;
	
	if (yaw > 360.0f)
		yaw -= 360.0f;

	float frameSpeed = 10;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		glm::mat4 posM = glm::rotate(posM, glm::radians(yaw), glm::vec3(0, 1, 0));
		glm::vec3 posDif = (posM * glm::vec4(0, 0, -1, 1) * frameSpeed);
		position += posDif;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		glm::mat4 posM = glm::rotate(posM, glm::radians(yaw), glm::vec3(0, 1, 0));
		glm::vec3 posDif = (posM * glm::vec4(0, 0, -1, 1) * frameSpeed);
		position += posDif;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		glm::mat4 posM = glm::rotate(posM, glm::radians(yaw), glm::vec3(0, 1, 0));
		glm::vec3 posDif = (posM * glm::vec4(-1, 0, 0, 1) * frameSpeed);
		position += posDif;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		glm::mat4 posM = glm::rotate(posM, glm::radians(yaw), glm::vec3(0, 1, 0));
		glm::vec3 posDif = (posM * glm::vec4(-1, 0, 0, 1) * frameSpeed);
		position -= posDif;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		position.y += frameSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		position.y -= frameSpeed;
	}
}

glm::mat4 PerspectiveCamera::BuildViewMatrix() const {
	glm::mat4 m1 = glm::rotate(m1, glm::radians(-pitch), glm::vec3(1, 0, 0));
	glm::mat4 m2 = glm::rotate(m2, glm::radians(-yaw), glm::vec3(0, 1, 0));
	glm::mat4 m3 = glm::translate(m3, -position);
	return m1 * m2 * m3;
}

glm::mat4 PerspectiveCamera::BuildProjectionMatrix() const {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	float aspect = (float)width / (float)height;

	return glm::perspective(fieldOfView, aspect, nearPlane, farPlane);
}
