#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator
{
public:
	GravityForceGenerator(float gravityAceleration);
	virtual void update(Particle* p);
	inline void changeGravity(float gravityAceleration) { aceleration = { aceleration.x, -gravityAceleration, aceleration.z }; }
	inline float getGravity() { return -aceleration.y; }

protected:
	Vector3 aceleration;
};

