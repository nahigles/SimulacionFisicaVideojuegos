#include "Particle.h"
#include <iostream>
#include <math.h>

// PUBLIC

// Constructor
Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acel, double damping, Vector4 color)
{
	pose = physx::PxTransform(pos);
	this->vel = vel;
	this->acel = acel;
	d = damping;
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1.0f)), &pose, color);
	alive = true;
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
	//std::cout << "Posicion: " << pose.p.x << ", " << pose.p.y << ", " << pose.p.z 
		//<< " Velocidad: " << vel.x << ", " << vel.y << ", " << vel.z << std::endl;

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
	//std::cout << "Posicion: " << pose.p.x << ", " << pose.p.y << ", " << pose.p.z
		//<< " Velocidad: " << vel.x << ", " << vel.y << ", " << vel.z << std::endl;

	// Dumping
	vel = vel * pow(d, t);
}
