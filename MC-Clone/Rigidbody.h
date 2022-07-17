#pragma once

#include <glm/glm.hpp>

class Rigidbody {
public:
	Rigidbody(glm::vec3 position);
	~Rigidbody() {};

	glm::vec3 GetPosition() { return position; }

	void IntegrateAcceleration(float dt);
	void IntegrateVelocity(float dt);

	void AddForce(glm::vec3 additionalForce) { force += additionalForce; }
	void ClearForce() { force = glm::vec3(0); }

	void ApplyLinearImpulse() { linearVelocity += force * inverseMass; }

protected:

	float inverseMass;
	float damping;

	glm::vec3 gravity;

	glm::vec3 linearVelocity;
	glm::vec3 force;

	glm::vec3 position;

	bool applyGravity;
};