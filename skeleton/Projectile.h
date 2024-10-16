#pragma once
#include "Particle.h"
class Projectile : public Particle
{
private:
	double masa;

public:
	Projectile(Vector3 pos, Vector3 vel, Vector3 acel, double damping, Vector4 color, double m, float size);
};

