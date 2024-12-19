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
#include "RigidSolidSystem.h"

using namespace physx;
using namespace std;

enum State
{
	START,
	E_FLOTACION,
	E_COLUMNA_AIRE,
	E_BOLAS_LOKAS,
	E_EXPLOSION_MOTOR,
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

	bool _pause = false;

	RenderItem* xRenderItem = NULL, * yRenderItem = NULL, * zRenderItem = NULL, * originRenderItem = NULL;
	PxTransform x, y, z, origin;

	Particle* pelota = nullptr; // Experimento pelota de aire

	RenderItem* baseRenderItem = NULL, * tapaRenderItem = NULL, * aireRenderItem = NULL;
	PxTransform base, tapa, aire;

	list<Particle*> particulas; // Particulas

	list<Projectile*> projectiles; // LISTA PROYECTILES

	list<ParticleSystem*> particleSystems; // SISTEMA DE PARTICULAS

	list<RigidSolidSystem*> rigidSolidSystems; // SISTEMA DE SOLIDOS RIGIDOS

	// Solido Rigido
	list<RigidSolid*> rigidSolids;

	// Gravedad
	GravityForceGenerator* gravityForceGenerator = nullptr;
	GravityForceGenerator* gravityForceGenerator2 = nullptr;
	GravityForceGenerator* gravityForceGenerator3 = nullptr;

	// Otras fuerzas
	WindForceGenerator* windForceGenerator = nullptr;
	WindForceGenerator* windForceGeneratorPelota = nullptr;
	WindForceGenerator* windForceGeneratorBolas1 = nullptr;
	WindForceGenerator* windForceGeneratorBolas2 = nullptr;
	WindForceGenerator* windForceGeneratorBolas3 = nullptr;
	WindForceGenerator* windForceGeneratorBolas4 = nullptr;
	TornadoForceGenerator* tornadoForceGenerator = nullptr;
	BlastForceGenerator* blastForceGenerator = nullptr;
	SpringForceGenerator* springForceGenerator1 = nullptr;
	SpringForceGenerator* springForceGenerator2 = nullptr;
	AnchoredSpringForceGenerator* anchoredSpringForceGenerator = nullptr;
	BouyancyForceGenerator* bouyancyForceGenerator = nullptr;

	// Base experimento Bolas lokas
	PxRigidStatic* baseEstatica = nullptr;
	PxShape* baseEstaticaShape = nullptr;
	RenderItem* baseEstaticaRenderItem = nullptr;

	

public:
	Game(physx::PxPhysics* physics, physx::PxScene* scene);
	void nextState();
	void previousState();
	void update(double t);
	void keyPressed(unsigned char key);
	~Game();

protected:
	void deleteAll();
	void deleteForces();
	void deleteSpecificForces();
	void createCircleOfParticles(Vector3 centerPosition);
	void updateElements(double t);
	void experimentoColumnaAireInit();
	void experimentoFlotacionInit();
	void experimentoBolasLokasInit();
};

