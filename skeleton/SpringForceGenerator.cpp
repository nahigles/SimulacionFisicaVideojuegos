#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(double k, double resting_length, Particle* other) :
	ForceGenerator(), _k(k), _resting_length(resting_length), _other(other) {}

void SpringForceGenerator::update(Particle* particle, double t) {
	Vector3 relative_pos_vector = _other->getPos() - particle->getPos();

	const float length = relative_pos_vector.normalize();
	const float delta_x = length - _resting_length;

	force = relative_pos_vector * delta_x * _k;

	particle->addForce(force);
}

