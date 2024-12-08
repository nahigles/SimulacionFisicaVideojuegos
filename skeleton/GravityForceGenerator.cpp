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

void GravityForceGenerator::update(RigidSolid* rs, double t)
{
	if (usingBounding && boundingShape->isInside(rs->getPosition()) || !usingBounding) {
		calculateForces(rs);
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

void GravityForceGenerator::calculateForces(RigidSolid* rs)
{
	force = rs->getMass() * aceleration;
	rs->addForce(force);
}
