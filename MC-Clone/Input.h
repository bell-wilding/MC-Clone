#pragma once

#include <map>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Input {
public:
	enum KeyVal {
		ONE, TWO, THREE, FOUR, FIVE, 
		SIX, SEVEN, EIGHT, NINE, ZERO,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,
		LEFT_SHIFT, LEFT_CONTROL, SPACE,
		TAB, ESCAPE,
		LAST
	};

	enum KeyState {
		PRESSED,
		DOWN,
		RELEASED
	};

	enum MouseButton {
		LEFT,
		RIGHT,
		MIDDLE
	};


	Input(GLFWwindow* window);
	~Input() {};

	void Update();

	bool GetKeyDown(KeyVal key) {
		return keyStates[key] == KeyState::DOWN;
	}

	bool GetKeyPressed(KeyVal key) {
		return keyStates[key] == KeyState::PRESSED;
	}

	bool GetMouseButtonDown(MouseButton button) {
		switch (button) {
		case LEFT:
			return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
		case RIGHT:
			return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
		case MIDDLE:
			return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
		}
	}

	glm::vec2 GetCursorPosition() {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return glm::vec2(x, y);
	}

	glm::ivec2 GetWindowSize() {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		return glm::ivec2(width, height);
	}

	void ShowCursor(bool on) {
		glfwSetInputMode(window, GLFW_CURSOR, on ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

protected:

	GLFWwindow* window;
	std::map<KeyVal, KeyState> keyStates;

	int keysGLFW[54] = {
		GLFW_KEY_1, GLFW_KEY_2,
		GLFW_KEY_3, GLFW_KEY_4,
		GLFW_KEY_5, GLFW_KEY_6,
		GLFW_KEY_7, GLFW_KEY_8,
		GLFW_KEY_9, GLFW_KEY_0,
		GLFW_KEY_F1, GLFW_KEY_F2,
		GLFW_KEY_F3, GLFW_KEY_F4,
		GLFW_KEY_F5, GLFW_KEY_F6,
		GLFW_KEY_F7, GLFW_KEY_F8,
		GLFW_KEY_F9, GLFW_KEY_F10,
		GLFW_KEY_F11, GLFW_KEY_F12,
		GLFW_KEY_Q, GLFW_KEY_W, 
		GLFW_KEY_E, GLFW_KEY_R, 
		GLFW_KEY_T, GLFW_KEY_Y, 
		GLFW_KEY_U, GLFW_KEY_I, 
		GLFW_KEY_O, GLFW_KEY_P,
		GLFW_KEY_A, GLFW_KEY_S, 
		GLFW_KEY_D, GLFW_KEY_F, 
		GLFW_KEY_G, GLFW_KEY_H, 
		GLFW_KEY_J, GLFW_KEY_K, 
		GLFW_KEY_L, GLFW_KEY_Z, 
		GLFW_KEY_X, GLFW_KEY_C, 
		GLFW_KEY_V, GLFW_KEY_B, 
		GLFW_KEY_N, GLFW_KEY_M,
		GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_CONTROL, 
		GLFW_KEY_SPACE, GLFW_KEY_TAB,
		GLFW_KEY_ESCAPE, -1
	};
};