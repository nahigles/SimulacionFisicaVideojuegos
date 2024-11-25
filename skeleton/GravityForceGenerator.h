#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator
{
protected:
	Vector3 aceleration;
	bool usingBounding;
public:
	GravityForceGenerator(float gravityAceleration, Vector3 p1 = { 0.0f,0.0f,0.0f }, Vector3 p2 = {0.0f,0.0f,0.0f}, float r = 0);
	virtual void update(Particle* p);
	inline void changeGravity(float gravityAceleration) { aceleration = { aceleration.x, -gravityAceleration, aceleration.z }; }
	inline float getGravity() { return -aceleration.y; }
	~GravityForceGenerator();

protected:
	void calculateForces(Particle* p);
};

