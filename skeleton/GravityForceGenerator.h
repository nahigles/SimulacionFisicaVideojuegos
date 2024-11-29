#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator
{
protected:
	Vector3 aceleration;
public:
	GravityForceGenerator(float gravityAceleration, Vector3 p1 = { 0.0f,0.0f,0.0f }, Vector3 p2 = {0.0f,0.0f,0.0f}, float r = 0, bool visible = true);
	virtual void update(Particle* p, double t);
	inline void changeGravity(float gravityAceleration) { aceleration = { aceleration.x, -gravityAceleration, aceleration.z }; }
	inline float getGravity() { return -aceleration.y; }
	~GravityForceGenerator();

protected:
	void calculateForces(Particle* p);
};

