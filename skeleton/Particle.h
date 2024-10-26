#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, double damping, Vector4 color, float size);
	Particle() {};
	~Particle();

	bool isAlive();
	inline double getLifeTime() const{ return lifeTime; }
	void update(double t);

protected:
	Vector3 vel;
	Vector3 acel;
	Vector3 force;
	physx::PxTransform pose;
	RenderItem* renderItem;
	double d = 0.98; // dumping (Entre 0 y 1)
	bool alive;
	double lifeTime;


	// METODOS
	// Si devuelve booleano que te diga si la particula esta viva o no
	void integrate(double t); // Euler
	void integrateSemi(double t); // Euler semiimplicito
};

