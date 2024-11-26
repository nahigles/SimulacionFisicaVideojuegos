#include "WindForceGenerator.h"

WindForceGenerator::WindForceGenerator(Vector3 vel, float k1, float k2, Vector3 p1, Vector3 p2, float r) : ForceGenerator(p1, p2, r)
{
	this->k1 = k1;
	this->k2 = k2;
	velocity = vel;
}

void WindForceGenerator::update(Particle* p)
{
	if (usingBounding && boundingShape->isInside(p->getPos()) || !usingBounding) {
		Vector3 velocityDifference = velocity - p->getVel();
		force = k1 * velocityDifference + k2 * velocityDifference.magnitude() * velocityDifference;
		p->addForce(force);
	}
}
