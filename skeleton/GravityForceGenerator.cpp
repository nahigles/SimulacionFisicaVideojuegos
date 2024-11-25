#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(float gravityAceleration, Vector3 p1, Vector3 p2, float r) :ForceGenerator(p1,p2,r)
{
	aceleration = Vector3(0, -gravityAceleration, 0);

}

void GravityForceGenerator::update(Particle* p)
{
	if (usingBounding && boundingShape->isInside(p->getPos()) || !usingBounding) {
		calculateForces(p);
	}
}

GravityForceGenerator::~GravityForceGenerator()
{

}

void GravityForceGenerator::calculateForces(Particle* p)
{
	// Calcula las fuerzas y las añade a la particula
	force = p->getMass() * aceleration;
	p->addForce(force);
}
