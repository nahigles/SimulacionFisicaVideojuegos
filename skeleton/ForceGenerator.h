#pragma once
#include "core.hpp"
#include "Particle.h"

enum Force
{
	GRAVITY,
	TORNADO,
	BLAST,
	LAST_FORCE
};

class ForceGenerator
{
protected:
	Vector3 force;

public:
	ForceGenerator();
	virtual void update(Particle* p) = 0;
};

