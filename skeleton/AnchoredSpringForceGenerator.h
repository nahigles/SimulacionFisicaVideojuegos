#pragma once
#include "SpringForceGenerator.h"
class AnchoredSpringForceGenerator : public SpringForceGenerator
{
public:
	AnchoredSpringForceGenerator(double k, double resting_length, const Vector3& anchored_pos) :
		SpringForceGenerator(k, resting_length, nullptr) {
		_other = new Particle(anchored_pos, { 0,0,0 }, {0,0,0}, 0.98, {0,0,0,1},1,1, CUBO);
	};

	~AnchoredSpringForceGenerator() { delete _other; }
};

