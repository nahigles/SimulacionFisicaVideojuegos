#include "WindForceGenerator.h"
#include <iostream>

WindForceGenerator::WindForceGenerator(Vector3 vel, float k1, float k2, Vector3 p1, Vector3 p2, float r, bool visible) : ForceGenerator(p1, p2, r, visible)
{
	this->k1 = k1;
	this->k2 = k2;
	velocity = vel;
}

void WindForceGenerator::update(Particle* p, double t)
{
	if (usingBounding && boundingShape->isInside(p->getPos()) || !usingBounding) {
		calculateForce(p);
	}
}

void WindForceGenerator::update(RigidSolid* rs, double t)
{
	if (usingBounding && boundingShape->isInside(rs->getPosition()) || !usingBounding) {
		calculateForce(rs);
	}
}

void WindForceGenerator::calculateForce(Particle* p)
{
	Vector3 velocityDifference = velocity - p->getVel();
	force = k1 * velocityDifference + k2 * velocityDifference.magnitude() * velocityDifference;
	p->addForce(force);
}

void WindForceGenerator::calculateForce(RigidSolid* rs)
{
	Vector3 velocityDifference = velocity - rs->getVelocity();
	force = k1 * velocityDifference + k2 * velocityDifference.magnitude() * velocityDifference;
	rs->addForce(force);
}
