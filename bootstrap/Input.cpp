#include "Input.h"
#include <GLFW/glfw3.h>

namespace aie {

Input* Input::m_instance = nullptr;

Input::Input() {
/*	auto KeyPressCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if ( action == GLFW_PRESS )
			Input::getInstance()->onKeyPressed(key);
		if ( action == GLFW_RELEASE )
			Input::getInstance()->onKeyReleased(key);
		if ( action == GLFW_REPEAT )
			Input::getInstance()->onKeyRepeate(key);
	};

	auto CharacterInputCallback = [](GLFWwindow* window, unsigned int character) {
		Input::getInstance()->onCharInput(character);
	};

	auto MouseInputCallback = [](GLFWwindow* window, int button, int action, int mods) {
		if (action == GLFW_PRESS)
			Input::getInstance()->onMousePressed(button);
		if (action == GLFW_RELEASE)
			Input::getInstance()->onMouseReleased(button);
	};

	auto MouseMoveCallback = [](GLFWwindow* window, double x, double y) {
		Input::getInstance()->onMouseMove((int)x, (int)y);
	};
	*/
	auto KeyPressCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {

		if (action == GLFW_PRESS)
			Input::getInstance()->onKeyPressed(key);
		else if (action == GLFW_RELEASE)
			Input::getInstance()->onKeyReleased(key);
		else if (action == GLFW_REPEAT)
			Input::getInstance()->onKeyRepeate(key);

		for (auto& f : Input::getInstance()->m_keyCallbacks)
			f(window, key, scancode, action, mods);
	};

	auto CharacterInputCallback = [](GLFWwindow* window, unsigned int character) {

		Input::getInstance()->onCharInput(character);

		for (auto& f : Input::getInstance()->m_charCallbacks)
			f(window, character);
	};

	auto MouseMoveCallback = [](GLFWwindow* window, double x, double y) {

		Input::getInstance()->onMouseMove((int)x, (int)y);

		for (auto& f : Input::getInstance()->m_mouseMoveCallbacks)
			f(window, x, y);
	};

	auto MouseInputCallback = [](GLFWwindow* window, int button, int action, int mods) {

		if (action == GLFW_PRESS)
			Input::getInstance()->onMousePressed(button);
		else if (action == GLFW_RELEASE)
			Input::getInstance()->onMouseReleased(button);

		for (auto& f : Input::getInstance()->m_mouseButtonCallbacks)
			f(window, button, action, mods);
	};

	auto MouseScrollCallback = [](GLFWwindow* window, double xoffset, double yoffset) {
		for (auto& f : Input::getInstance()->m_mouseScrollCallbacks)
			f(window, xoffset, yoffset);
	};

	GLFWwindow* window = glfwGetCurrentContext();
	glfwSetKeyCallback(window, KeyPressCallback);
	glfwSetCharCallback(window, CharacterInputCallback);
	glfwSetMouseButtonCallback(window, MouseInputCallback);
	glfwSetCursorPosCallback(window, MouseMoveCallback);
	glfwSetScrollCallback(window, MouseScrollCallback);
	
	m_mouseX = 0;
	m_mouseY = 0;
	m_mouseScroll = 0;
}

Input::~Input() {

}

void Input::onKeyPressed(int keyID) {
	m_keyStatus[keyID] = JUST_PRESSED;
	m_keysToUpdate.push_back(keyID);
	m_pressedKeys.push_back(keyID);
}

void Input::onKeyReleased(int keyID) {
	m_keyStatus[keyID] = JUST_RELEASED;
	m_keysToUpdate.push_back(keyID);
}

void Input::onKeyRepeate(int key) {
	m_pressedKeys.push_back(key);
}

void Input::onCharInput(unsigned int character) {
	m_pressedCharacters.push_back(character);
}

void Input::onMouseMove(int newXPos, int newYPos) {
	m_mouseX = newXPos;
	m_mouseY = newYPos;
}

void Input::onMousePressed(int mouseButtonID) {
	m_mouseState[mouseButtonID] = JUST_PRESSED;
	m_mouseToUpdate.push_back(mouseButtonID);
}

void Input::onMouseReleased(int mouseButtonID) {
	m_mouseState[mouseButtonID] = JUST_RELEASED;
	m_mouseToUpdate.push_back(mouseButtonID);
}

void Input::update() {
	for (auto& key : m_keysToUpdate)
		key += 1;

	for (auto& state : m_mouseToUpdate)
		state += 1;

	m_mouseToUpdate.clear();
	m_keysToUpdate.clear();

	m_pressedKeys.clear();
	m_pressedCharacters.clear();
}

bool Input::isKeyDown(int inputKeyID) {
	return m_keyStatus[inputKeyID] == JUST_PRESSED || m_keyStatus[inputKeyID] == DOWN;
}

bool Input::isKeyUp(int inputKeyID) {
	return m_keyStatus[inputKeyID] == JUST_RELEASED || m_keyStatus[inputKeyID] == UP;
}

bool Input::wasKeyPressed(int inputKeyID) {
	return m_keyStatus[inputKeyID] == JUST_PRESSED;
}

bool Input::wasKeyReleased(int inputKeyID) {
	return m_keyStatus[inputKeyID] == JUST_RELEASED;
}

const std::vector<unsigned int> &Input::getPressedKeys() const {
	return m_pressedKeys;
}

const std::vector<unsigned int> &Input::getPressedCharacters() const {
	return m_pressedCharacters;
}

bool Input::isMouseButtonDown(int inputMouseID) {
	return m_mouseState[inputMouseID] == DOWN;
}

bool Input::isMouseButtonUp(int inputMouseID) {
	return m_mouseState[inputMouseID] == UP || m_mouseState[inputMouseID] == JUST_RELEASED;
}

bool Input::wasMouseButtonPressed(int inputMouseID) {
	return m_mouseState[inputMouseID] == JUST_PRESSED;
}

bool Input::wasMouseButtonReleased(int inputMouseID) {
	return m_mouseState[inputMouseID] == JUST_RELEASED;
}

int Input::getMouseX() {
	return m_mouseX;
}

int Input::getMouseY() {
	return m_mouseY;
}

int Input::getMouseScroll() {
	return m_mouseScroll;
}

void Input::getMouseXY(int* x, int* y) {
	if ( x != nullptr ) *x = m_mouseX;
	if ( y != nullptr) *y = m_mouseY;
}

} // namespace aie