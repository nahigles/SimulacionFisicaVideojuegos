#pragma once
#include "ForceGenerator.h"
class BlastForceGenerator : public ForceGenerator
{
protected:
	const float timeConst = 20.0f;

	float k;
	float radio;
	Vector3 centerPos;
	double time;

	bool active;

public:
	BlastForceGenerator(float intensity, float blastRadio, Vector3 center, float r = 0, bool visible = true);
	virtual void update(Particle* p, double t);
	void activateBlast();
};

