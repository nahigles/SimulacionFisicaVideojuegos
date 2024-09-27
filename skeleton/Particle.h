#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, double damping);
	Particle() {};
	~Particle();

	void integrate(double t); // Euler
	void integrateSemi(double t); // Euler semiimplicito

private:
	Vector3 vel;
	Vector3 acel;
	physx::PxTransform pose;
	RenderItem* renderItem;
	double d = 0.98; // dumping (Entre 0 y 1)
};

