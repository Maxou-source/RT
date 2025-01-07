#include "AObject.hpp"

int AObject::idCounter = 0;

AObject::AObject() {
	id = ++idCounter;
	center = 0;
}