#include "Rigidbody.h"

Rigidbody::Rigidbody(glm::vec3 position) : position(position) {
	gravity = glm::vec3(0, -45, 0);
	inverseMass = 1.0f;
	damping = glm::vec3(8.0f, 1.0f, 8.0f);
	applyGravity = true;
	elasticity = 0.5f;
}

void Rigidbody::IntegrateAcceleration(float dt) {
	glm::vec3 acceleration = force * inverseMass;
	if (applyGravity) acceleration += gravity;
	linearVelocity += acceleration * dt;
}

void Rigidbody::IntegrateVelocity(float dt) {
	glm::vec3 frameLinearDamping = glm::vec3(1, 1, 1) - (damping * dt);
	position += linearVelocity * dt;
	linearVelocity *= frameLinearDamping;
}
