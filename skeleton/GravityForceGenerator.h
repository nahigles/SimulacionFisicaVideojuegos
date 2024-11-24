#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator
{
public:
	GravityForceGenerator(float gravityAceleration);
	virtual void update(Particle* p);

protected:
	Vector3 aceleration;
};

