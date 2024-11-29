#pragma once
#include "RenderUtils.hpp"
#include "Particle.h"
#include "Projectile.h"
#include "ParticleSystem.h"
#include <list>
#include <iostream>
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "TornadoForceGenerator.h"
#include "BlastForceGenerator.h"

using namespace physx;
using namespace std;

enum State
{
	START = 0,
	GAME,
	FORCES,
	END,
	LAST_STATE

};

class Game
{
protected:
	State _state;
	State _nextState;

	RenderItem* xRenderItem = NULL, * yRenderItem = NULL, * zRenderItem = NULL, * originRenderItem = NULL;
	PxTransform x, y, z, origin;

	//Particle* particle = nullptr;
	//Particle* particleG1 = nullptr;
	//Particle* particleG2 = nullptr;

	list<Particle*> particulas;

	list<Projectile*> projectiles; // LISTA PROYECTILES

	list<ParticleSystem*> particleSystems; // SISTEMA DE PARTICULAS

	// Gravedad
	GravityForceGenerator* gravityForceGenerator = nullptr;
	GravityForceGenerator* gravityForceGenerator2 = nullptr;
	GravityForceGenerator* gravityForceGenerator3 = nullptr;

	WindForceGenerator* windForceGenerator = nullptr;
	TornadoForceGenerator* tornadoForceGenerator = nullptr;
	BlastForceGenerator* blastForceGenerator = nullptr;

public:
	Game();
	void nextState();
	void update(double t);
	void keyPressed(unsigned char key);
	~Game();

protected:
	void deleteAll();
	void deleteForces();
	void createCircleOfParticles(Vector3 centerPosition);
};

