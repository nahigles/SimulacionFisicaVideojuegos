#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, double damping, Vector4 color);
	Particle() {};
	~Particle();

	// Si devuelve booleano que te diga si la particula esta viva o no
	void integrate(double t); // Euler
	void integrateSemi(double t); // Euler semiimplicito
	bool isAlive();

protected:
	Vector3 vel;
	Vector3 acel;
	physx::PxTransform pose;
	RenderItem* renderItem;
	double d = 0.98; // dumping (Entre 0 y 1)
	bool alive;
};

