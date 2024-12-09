#include "TornadoForceGenerator.h"

TornadoForceGenerator::TornadoForceGenerator(float k, float kTornado, Vector3 center, float r, bool visible):WindForceGenerator({ 0.0f,0.0f,0.0f },k, 0,center, { 0.0f,0.0f,0.0f }, r, visible)
{
	centerPos = center;
	this->k = kTornado;
}

void TornadoForceGenerator::update(Particle* p, double t)
{
	Vector3 particlePos = p->getPos();
	if (usingBounding && boundingShape->isInside(particlePos) || !usingBounding) {

		// Cambio velocidad dependiendo de la posicion de la particula
		velocity = k * Vector3(-(particlePos.z - centerPos.z), 50 - (particlePos.y - centerPos.y), particlePos.x - centerPos.x);
		calculateForce(p); // Calcula con clase padre viento
	}
}

void TornadoForceGenerator::update(RigidSolid* rs, double t)
{
	Vector3 rigidSolidPos = rs->getPosition();
	if (usingBounding && boundingShape->isInside(rigidSolidPos) || !usingBounding) {

		// Cambio velocidad dependiendo de la posicion del solido
		velocity = k * Vector3(-(rigidSolidPos.z - centerPos.z), 50 - (rigidSolidPos.y - centerPos.y), rigidSolidPos.x - centerPos.x);
		calculateForce(rs); // Calcula con clase padre viento
	}
}
