#include "Input.h"

Input::Input(GLFWwindow* window) : window(window) {
	for (int key = KeyVal::ONE; key != KeyVal::LAST; ++key) {
		keyStates[(KeyVal)key] = KeyState::RELEASED;
	}
}

void Input::Update() {
	for (int key = KeyVal::ONE; key != KeyVal::LAST; ++key) {
		KeyVal val = (KeyVal)key;
		bool previouslyDown = keyStates[val] == KeyState::DOWN || keyStates[val] == KeyState::PRESSED;
		bool keyPressed = glfwGetKey(window, keysGLFW[key]) == GLFW_PRESS;
		keyStates[val] = keyPressed ? (previouslyDown ? KeyState::DOWN : KeyState::PRESSED) : (KeyState::RELEASED);
	}
}
