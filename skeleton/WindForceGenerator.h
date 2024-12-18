#pragma once
#include "ForceGenerator.h"
class WindForceGenerator : public ForceGenerator
{
protected:
	float k1;
	float k2;
	Vector3 velocity;

public:
	WindForceGenerator(Vector3 vel, float k1, float k2 = 0, Vector3 p1 = { 0.0f,0.0f,0.0f }, Vector3 p2 = { 0.0f,0.0f,0.0f }, float r = 0, bool visible = true);
	virtual void update(Particle* p, double t);
	virtual void update(RigidSolid* rs, double t);

protected:
	virtual void calculateForce(Particle* p);
	virtual void calculateForce(RigidSolid* rs);
};

