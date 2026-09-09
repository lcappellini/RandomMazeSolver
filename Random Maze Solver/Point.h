#pragma once

class Point {
public:
	int x;
	int y;

	bool operator==(const Point& b) {
		return x == b.x && y == b.y;
	}
};