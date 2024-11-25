#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(float gravityAceleration, Vector3 p1, Vector3 p2, float r) :ForceGenerator()
{
	aceleration = Vector3(0, -gravityAceleration, 0);

	Vector3 zeroVector = { 0.0f,0.0f,0.0f };
	// Solo se crean boundings si se ha añadido algun dato para los limites
	if (!(p1 == zeroVector && p2 == zeroVector && r == 0)) {
		usingBounding = true;

		// SI radio no es 0 --> Hacemos esfera
		if (r) {
			boundingShape = new BoundingSphere(p1, r);
		}
		// Sino caja
		else {
			boundingShape = new BoundingBox(p1, p2);
		}
	}
	else
		usingBounding = false;
}

void GravityForceGenerator::update(Particle* p)
{
	if (usingBounding && boundingShape->isInside(p->getPos()) || !usingBounding) {
		calculateForces(p);
	}
}

GravityForceGenerator::~GravityForceGenerator()
{
	if (boundingShape != nullptr) {
		delete boundingShape;
		boundingShape = nullptr;
	}
}

void GravityForceGenerator::calculateForces(Particle* p)
{
	// Calcula las fuerzas y las añade a la particula
	force = p->getMass() * aceleration;
	p->addForce(force);
}
