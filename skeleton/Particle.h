#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>

class ForceGenerator;

enum Forma {
	CUBO,
	ESFERA
};

class Particle
{
public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, double damping, Vector4 color, float size, double m, Forma f = ESFERA);
	Particle() {};
	~Particle();

	bool isAlive();
	inline double getLifeTime() const { return lifeTime; }
	void update(double t);
	void addForceGenerator(ForceGenerator* force);
	void addForce(Vector3 f);
	inline double getMass() const { return masa; }
	inline double getInverseMass() const { if (masa)return (1.0 / masa); else return 0.0; }
	inline void clearForces() { forceAcum = Vector3(0, 0, 0); }
	inline Vector3 getPos() { return pose.p; }
	inline Vector3 getVel() { return vel; }
	inline double getHeight() { return size * 2; } // ALto
	inline double getVolume() { return size * 2 * size * 2 * size * 2; } // Ancho * Largo * Alto (En este caso es un cubo asiq son iguales)

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
	double size;

	// Fuerzas
	std::list<ForceGenerator*> forces;

	// METODOS
	// Si devuelve booleano que te diga si la particula esta viva o no
	void integrate(double t); // Euler
	void integrateSemi(double t); // Euler semiimplicito
	void updateForces(double t);
};

