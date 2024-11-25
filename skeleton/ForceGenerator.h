#pragma once
#include "core.hpp"
#include "Particle.h"
//#include "Bounding.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

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
	Shape shape;
	Bounding* boundingShape = nullptr;

public:
	ForceGenerator();
	virtual void update(Particle* p) = 0;
};

