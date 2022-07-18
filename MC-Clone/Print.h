#pragma once

#include <glm/glm.hpp>
#include <iostream>

class Print {
public:
	static void Vector3(glm::vec3 vec) {
		std::cout << vec.x << " : " << vec.y << " : " << vec.z << std::endl;
	}
};