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
#include "SpringForceGenerator.h"
#include "AnchoredSpringForceGenerator.h"
#include "BouyancyForceGenerator.h"
#include "RigidSolid.h"

using namespace physx;
using namespace std;

enum State
{
	START = 0,
	GAME,
	FORCES,
	RIGID_SOLID,
	END,
	LAST_STATE

};


class Game
{
protected:
	physx::PxPhysics* gPhysics = nullptr;
	physx::PxScene* gScene = nullptr;
	
	State _state;
	State _nextState;

	RenderItem* xRenderItem = NULL, * yRenderItem = NULL, * zRenderItem = NULL, * originRenderItem = NULL;
	PxTransform x, y, z, origin;


	list<Particle*> particulas; // Particulas

	list<Projectile*> projectiles; // LISTA PROYECTILES

	list<ParticleSystem*> particleSystems; // SISTEMA DE PARTICULAS

	// Gravedad
	GravityForceGenerator* gravityForceGenerator = nullptr;
	GravityForceGenerator* gravityForceGenerator2 = nullptr;
	GravityForceGenerator* gravityForceGenerator3 = nullptr;

	// Otras fuerzas
	WindForceGenerator* windForceGenerator = nullptr;
	TornadoForceGenerator* tornadoForceGenerator = nullptr;
	BlastForceGenerator* blastForceGenerator = nullptr;
	SpringForceGenerator* springForceGenerator1 = nullptr;
	SpringForceGenerator* springForceGenerator2 = nullptr;
	AnchoredSpringForceGenerator* anchoredSpringForceGenerator = nullptr;
	BouyancyForceGenerator* bouyancyForceGenerator = nullptr;

	// Solido Rigido
	RigidSolid* rigidSolid = nullptr;

public:
	Game(physx::PxPhysics* physics, physx::PxScene* scene);
	void nextState();
	void update(double t);
	void keyPressed(unsigned char key);
	~Game();

protected:
	void deleteAll();
	void deleteForces();
	void createCircleOfParticles(Vector3 centerPosition);
};

