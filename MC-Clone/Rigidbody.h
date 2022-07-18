#pragma once

#include <glm/glm.hpp>

class Rigidbody {
public:
	Rigidbody(glm::vec3 position);
	~Rigidbody() {};

	glm::vec3 GetPosition() { return position; }
	void SetPosition(glm::vec3 newPos) { position = newPos; }

	glm::vec3 GetLinearVelocity() { return linearVelocity; }
	void SetLinearVelocity(glm::vec3 linVelocity) { linearVelocity = linVelocity; }

	void SetDamping(glm::vec3 newDamping) { damping = newDamping; }

	float GetInverseMass() { return inverseMass; }
	float GetElasticity() { return elasticity; }

	void IntegrateAcceleration(float dt);
	void IntegrateVelocity(float dt);

	void AddForce(glm::vec3 additionalForce) { force += additionalForce; }
	void ClearForce() { force = glm::vec3(0); }

	void ApplyLinearImpulse(glm::vec3 force) { linearVelocity += force * inverseMass; }

	void SetApplyGravity(bool gravityOn) { applyGravity = gravityOn; }

protected:

	float inverseMass;
	glm::vec3 damping;

	glm::vec3 gravity;

	glm::vec3 linearVelocity;
	glm::vec3 force;

	glm::vec3 position;

	float elasticity;

	bool applyGravity;
};