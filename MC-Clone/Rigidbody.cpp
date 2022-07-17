#include "Rigidbody.h"

Rigidbody::Rigidbody(glm::vec3 position) : position(position) {
	gravity = glm::vec3(0, -30, 0);
	inverseMass = 1.0f;
	damping = 0.935f;
	applyGravity = false;
}

void Rigidbody::IntegrateAcceleration(float dt) {
	glm::vec3 acceleration = force * inverseMass;
	if (applyGravity) acceleration += gravity;
	linearVelocity += acceleration * dt;
}

void Rigidbody::IntegrateVelocity(float dt) {
	position += linearVelocity * dt;
	linearVelocity *= damping;
}
