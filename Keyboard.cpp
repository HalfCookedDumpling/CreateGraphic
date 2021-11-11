#include "Keyboard.h"
#include <cstring>

Keyboard::Keyboard () {

	memset(arr, 0, 255);

}

Keyboard::~Keyboard () {



}

Keyboard::Keyboard (const Keyboard &other) {



}

Keyboard * Keyboard::getKeyboard () {

	static Keyboard k;

	return &k;

}

void Keyboard::press (char key) {

	if (!arr[key]) {

		arr[key] = 1;

	}

}

void Keyboard::release (char key) {

	if (arr[key]) {

		arr[key] = 0;

	}

}

bool Keyboard::isPressed (char key) {

	if (arr[key] == 1) {

		arr[key] = 2;

		return true;

	}

	return false;

}

bool Keyboard::isDown (char key) const {

	if (arr[key]) {

		return true;

	}

	return false;

}
