#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>

class ForceGenerator;

class Particle
{
public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, double damping, Vector4 color, float size, double m);
	Particle() {};
	~Particle();

	bool isAlive();
	inline double getLifeTime() const { return lifeTime; }
	/*virtual*/ void update(double t);
	void addForceGenerator(ForceGenerator* force);
	void addForce(Vector3 f);
	inline double getMass() const { return masa; }
	inline double getInverseMass() const { return 1.0/masa; }
	inline void clearForces(){ forceAcum = Vector3(0, 0, 0); }

protected:
	Vector3 vel;
	Vector3 acel;
	Vector3 forceAcum;
	physx::PxTransform pose;
	RenderItem* renderItem;
	double d = 0.98; // dumping (Entre 0 y 1)
	bool alive;
	double lifeTime;
	double masa;

	// Fuerzas
	std::list<ForceGenerator*> forces;

	// METODOS
	// Si devuelve booleano que te diga si la particula esta viva o no
	void integrate(double t); // Euler
	void integrateSemi(double t); // Euler semiimplicito
	void updateForces();
};

