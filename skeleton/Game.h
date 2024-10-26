#pragma once
#include "RenderUtils.hpp"
#include "Particle.h"
#include "Projectile.h"
#include "ParticleSystem.h"
#include <list>
#include <iostream>

using namespace physx;
using namespace std;

enum State
{
	START = 0,
	GAME,
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

	Particle* particle;
	list<Projectile*> projectiles;

	list<ParticleSystem*> particleSystems; // SISTEMA DE PARTICULAS
public:
	Game();
	void nextState();
	void update(double t);
	void keyPressed(unsigned char key);
	~Game();

protected:
	void deleteAll();
};

