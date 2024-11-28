#include "TornadoForceGenerator.h"

TornadoForceGenerator::TornadoForceGenerator(Vector3 vel, float k, float kTornado, Vector3 center, float r, bool visible):WindForceGenerator(vel,k, 0,center, { 0.0f,0.0f,0.0f }, r, visible)
{
	centerPos = center;
	this->k = kTornado;
}

void TornadoForceGenerator::update(Particle* p)
{
	Vector3 particlePos = p->getPos();
	if (usingBounding && boundingShape->isInside(particlePos) || !usingBounding) {

		// Cambio velocidad dependiendo de la posicion de la particula
		velocity = k * Vector3(-(particlePos.z - centerPos.z), 50 - (particlePos.y - centerPos.y), particlePos.x - centerPos.x);
		calculateForce(p); // Calcula con clase padre viento
	}
}
