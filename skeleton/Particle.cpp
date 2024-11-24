#include "Particle.h"
#include <iostream>
#include <math.h>
#include "ForceGenerator.h"

// PUBLIC

// Constructor
Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acel, double damping, Vector4 color,  float size, double m)
{
	pose = physx::PxTransform(pos);
	this->vel = vel;
	this->acel = acel;
	d = damping;
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(size)), &pose, color);
	alive = true;
	lifeTime = 0;
	forceAcum = { 0.0,0.0,0.0 };
	masa = m;
}

// Destructor
Particle::~Particle() {
	DeregisterRenderItem(renderItem);
}

// Metodo Euler
void Particle::integrate(double t)
{
	// Actualizo posicion
	pose.p += t * vel;

	// Actualizo velocidad
	vel += t * acel;

	// Dumping
	vel = vel * pow(d, t);
}

void Particle::integrateSemi(double t)
{

	// Actualizo velocidad
	vel += t * acel;

	// Actualizo posicion
	pose.p += t * vel;

	// Dumping
	vel = vel * pow(d, t);
}

void Particle::updateForces()
{
	for (auto g : forces) {
		g->update(this);
	}
}

bool Particle::isAlive()
{
	if (pose.p.y <= 0)
		alive = false;

	return alive;
}

void Particle::update(double t)
{

	// update generadores
	updateForces();

	// Calcula aceleracion de la fuerza acumulada
	acel = forceAcum * getInverseMass();

	integrateSemi(t);

	// Pongo a 0 la fuerza
	clearForces();

	// Actualizo tiempo de vida
	lifeTime += t;
}

void Particle::addForceGenerator(ForceGenerator* force)
{
	forces.push_back(force);
}

void Particle::addForce(Vector3 f)
{
	forceAcum += f;
}
