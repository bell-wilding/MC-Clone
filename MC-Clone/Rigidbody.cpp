#include "Rigidbody.h"

Rigidbody::Rigidbody(glm::vec3 position) : position(position) {
	gravity = glm::vec3(0, -45, 0);
	inverseMass = 1.0f;
	damping = glm::vec3(0.925f, 0.9999999999f, 0.925f);
	applyGravity = true;
	elasticity = 0.175f;
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
