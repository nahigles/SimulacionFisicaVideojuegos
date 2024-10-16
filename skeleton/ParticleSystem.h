#pragma once
#include "core.hpp"
#include <list>
#include "Particle.h"
#include "RenderUtils.hpp"
#include <random>

class ParticleSystem
{
protected:
	const double randMin = -15;
	const double randMax = 15;
	double timeBetweenParticles;

	// Atributos
	Vector3 initialpos;
	Vector3 initialVel;
	int initialSize;
	Vector4 initialColor;
	//physx::PxGeometry initialShape;
	int initialLifeTime;

	// Lista para guardar particulas
	std::list<Particle*> particles;


	// Variables auxiliares
	double timeCont; // Contador del tiempo que ha pasado


	// METODOS
	void createParticle();

public:
	// Contructora
	ParticleSystem(Vector3 pos, Vector3 vel, int size, Vector4 color, double lifeTime, double timeBetween);
	~ParticleSystem();
	void update(double t);

};

