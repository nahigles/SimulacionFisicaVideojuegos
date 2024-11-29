#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(float gravityAceleration, Vector3 p1, Vector3 p2, float r, bool visible) :ForceGenerator(p1,p2,r, visible)
{
	aceleration = Vector3(0, -gravityAceleration, 0);

}

void GravityForceGenerator::update(Particle* p, double t)
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
	// Calcula las fuerzas y las a�ade a la particula
	force = p->getMass() * aceleration;
	p->addForce(force);
}
