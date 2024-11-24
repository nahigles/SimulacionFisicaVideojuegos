#include "Projectile.h"

Projectile::Projectile(Vector3 pos, Vector3 vel, Vector3 acel, double damping, Vector4 color, double m, float size) : Particle(pos, vel, acel, damping, color, size, m)
{
}

//void Projectile::update(double t)
//{
//
//	// update generadoresS
//	//updateForces();
//
//	//// Calcula aceleracion de la fuerza acumulada
//	//acel = forceAcum * getInverseMass();
//
//	integrateSemi(t);
//
//	//// Pongo a 0 la fuerza
//	//clearForces();
//
//	// Actualizo tiempo de vida
//	lifeTime += t;
//}
