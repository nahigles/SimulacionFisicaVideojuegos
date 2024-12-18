#pragma once
#include "core.hpp"
#include "Particle.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "RigidSolid.h"

class ForceGenerator
{
protected:
	Vector3 force;
	Bounding* boundingShape = nullptr;
	bool usingBounding;

public:
	ForceGenerator(Vector3 p1 = { 0.0f,0.0f,0.0f }, Vector3 p2 = { 0.0f,0.0f,0.0f }, float r = 0, bool visible = true);
	virtual void update(Particle* p, double t) = 0;
	virtual void update(RigidSolid* rs, double t) = 0;
	~ForceGenerator();
};

