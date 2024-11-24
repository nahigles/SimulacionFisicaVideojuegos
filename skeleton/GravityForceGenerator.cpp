#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(float gravityAceleration):ForceGenerator()
{
	aceleration = Vector3(0, -gravityAceleration, 0);
}

void GravityForceGenerator::update(Particle* p)
{
	force = p->getMass() * aceleration;
	p->addForce(force);
}
