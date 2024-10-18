#pragma once
#include "core.hpp"
#include <list>
#include "Particle.h"
#include "RenderUtils.hpp"
#include <random>

enum ParticleType {
	RAIN,
	BALLS,
	SNOW,
	FOAM,
	LAST_ELEM
};

class ParticleSystem
{

protected:
	double randMinVel = -15;
	double randMaxVel = 15;
	double randMinPos = -5;
	double randMaxPos = 5;
	double timeBetweenParticles;
	ParticleType type;

	// Atributos
	Vector3 initialpos;
	Vector3 initialVel;
	float initialSize;
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
	ParticleSystem(Vector3 pos, Vector3 vel, float size, Vector4 color, double lifeTime, double timeBetween, double randomPos, double randomVel, ParticleType pType);
	~ParticleSystem();
	void update(double t);

};

