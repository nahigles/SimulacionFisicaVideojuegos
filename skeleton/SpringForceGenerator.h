#pragma once
#include "ForceGenerator.h"
class SpringForceGenerator : public ForceGenerator
{
public:
	SpringForceGenerator(double k, double resting_length, Particle* other);

	virtual void update(Particle* particle, double t);
	virtual void update(RigidSolid* rs, double t) override {};

	inline void setK(double k) { _k = k; }

	virtual ~SpringForceGenerator(){}

protected:
	double _k; // Elastic Coeff.
	double _resting_length;
	Particle* _other;


};

