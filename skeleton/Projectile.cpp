#include "Projectile.h"

Projectile::Projectile(Vector3 pos, Vector3 vel, Vector3 acel, double damping, Vector4 color, double m) : Particle(pos, vel, acel, damping, color)
{
	masa = m;
}
