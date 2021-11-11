#include "Mouse.h"

Mouse::Mouse () {

	this->x = 0;

	this->y = 0;
	
}

Mouse::~Mouse () { }

Mouse::Mouse (Mouse *other) { }

Mouse *Mouse::getMouse () {

	static Mouse m;

	return &m;

}

int Mouse::getX () const {

	return (int) this->x;

}

int Mouse::getY () const {

	return (int) this->y;

}

int Mouse::getXTemp () const {

	return xTemp;

}

int Mouse::getYTemp () const {

	return yTemp;

}

bool Mouse::lButtonPressed () {

	if (lDown) {

		lDown++;

		return true;

	}

	return false;

}

bool Mouse::rButtonPressed () {

	if (rDown) {
	
		rDown++;

		return true;

	}

	return false;

}

void Mouse::setX (int x) {

	this->x = (double) x;

}

void Mouse::setY (int y) {

	this->y = (double) y;

}

void Mouse::setXTemp (int x) {

	this->xTemp = x;

}

void Mouse::setYTemp (int y) {

	this->yTemp = y;

}

void Mouse::set (int x, int y) {

	this->x = (double) x;

	this->y = (double) y;

}

void Mouse::setXYTemp (int x, int y) {

	this->xTemp = x;

	this->yTemp = y;

}

void Mouse::pressLButton () {

	lDown = 1;

}

void Mouse::pressRButton () {

	rDown = 1;

}

void Mouse::releaseLButton () {

	lDown = 0;

}

void Mouse::releaseRButton () {

	rDown = 0;

}
