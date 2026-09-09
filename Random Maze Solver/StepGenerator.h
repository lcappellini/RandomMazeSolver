#pragma once
#include <random>

class StepGenerator
{
public:
	StepGenerator(int16_t stepx, int16_t stepy) : distx(-stepx, stepx), disty(-stepy, stepy) {
		rng.seed(std::random_device{}());
	}

	int16_t getX() {
		return distx(rng);
	}

	int16_t getY() {
		return disty(rng);
	}

private:
	std::mt19937 rng;
	std::uniform_int_distribution<int16_t> distx;
	std::uniform_int_distribution<int16_t> disty;
};
