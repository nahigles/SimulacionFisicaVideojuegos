#pragma once
#include "WindForceGenerator.h"
class TornadoForceGenerator : public WindForceGenerator
{
protected:
	Vector3 centerPos;
	float k;

public:
	TornadoForceGenerator(Vector3 vel, float k, float kTornado, Vector3 center, float r = 0, bool visible = true);
	void update(Particle* p) override;
};

