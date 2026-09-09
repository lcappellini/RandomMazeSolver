#pragma once
#include <cstdint>
#include "Point.h"

class Rect {
public:
	int16_t top;
	int16_t left;
	int16_t bottom;
	int16_t right;

	bool inside(Point& p) {
		return top < p.y && p.y < bottom && left < p.x && p.x < right;
	}
};