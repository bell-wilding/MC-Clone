#include "Object.h"

#include <glm/gtc/matrix_transform.hpp>

Object::Object(glm::vec3 position, glm::vec3 scale, glm::vec3 orientation) 
	: position(position), scale(scale), orientation(orientation) {

	glm::mat4 pos = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 ori = ToMatrix(ToQuaternion(orientation));
	glm::mat4 sca = glm::scale(glm::mat4(1.0f), scale);

	worldMatrix = pos * ori * sca;
}

Object::~Object() {
}

glm::mat4 Object::ToMatrix(const glm::vec4 & o)
{
	glm::mat4 mat(1.0f);

	float yy = o.y * o.y;
	float zz = o.z * o.z;
	float xy = o.x * o.y;
	float zw = o.z * o.w;
	float xz = o.x * o.z;
	float yw = o.y *  o.w;
	float xx = o.x *  o.x;
	float yz = o.y *  o.z;
	float xw = o.x *  o.w;

	mat[0][0] = 1 - 2 * yy - 2 * zz;
	mat[0][1] = 2 * xy + 2 * zw;
	mat[0][2] = 2 * xz - 2 * yw;

	mat[1][0] = 2 * xy - 2 * zw;
	mat[1][1] = 1 - 2 * xx - 2 * zz;
	mat[1][2] = 2 * yz + 2 * xw;

	mat[2][0] = 2 * xz + 2 * yw;
	mat[2][1] = 2 * yz - 2 * xw;
	mat[2][2] = 1 - 2 * xx - 2 * yy;

	return mat;
}

glm::vec4 Object::ToQuaternion(const glm::vec3 & eulerOrientation) {
	float cos1 = (float)cos(glm::radians(eulerOrientation.x * 0.5f));
	float cos2 = (float)cos(glm::radians(eulerOrientation.y * 0.5f));
	float cos3 = (float)cos(glm::radians(eulerOrientation.z * 0.5f));

	float sin1 = (float)sin(glm::radians(eulerOrientation.x * 0.5f));
	float sin2 = (float)sin(glm::radians(eulerOrientation.y * 0.5f));
	float sin3 = (float)sin(glm::radians(eulerOrientation.z * 0.5f));

	glm::vec4 q;

	q.x = (sin1 * sin2 * cos3) + (cos1 * cos2 * sin3);
	q.y = (sin1 * cos2 * cos3) + (cos1 * sin2 * sin3);
	q.z = (cos1 * sin2 * cos3) - (sin1 * cos2 * sin3);
	q.w = (cos1 * cos2 * cos3) - (sin1 * sin2 * sin3);

	return q;
}
