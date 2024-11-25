#pragma once
#include "core.hpp"
#include "Particle.h"
//#include "Bounding.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

class ForceGenerator
{
protected:
	Vector3 force;
	Bounding* boundingShape = nullptr;
	bool usingBounding;

public:
	ForceGenerator(Vector3 p1 = { 0.0f,0.0f,0.0f }, Vector3 p2 = { 0.0f,0.0f,0.0f }, float r = 0);
	virtual void update(Particle* p) = 0;
	~ForceGenerator();
};

