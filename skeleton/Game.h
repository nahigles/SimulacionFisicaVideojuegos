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

extern std::string display_text;
extern std::string start_text;
extern std::string end_text;
extern std::string exp1_text;
extern std::string exp2_text;
extern std::string exp3_text;
extern std::string exp4_text;
extern std::string exp5_text;
extern std::string exp6_text;


enum State
{
	START,
	E_FLOTACION,
	E_COLUMNA_AIRE,
	E_QUIMICA,
	E_BOLAS_LOKAS,
	E_GRIFO_INFINITO,
	E_EXPLOSION_MOTOR,
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
	SpringForceGenerator* springForceGenerator3 = nullptr;
	SpringForceGenerator* springForceGenerator4 = nullptr;
	AnchoredSpringForceGenerator* anchoredSpringForceGenerator = nullptr;
	BouyancyForceGenerator* bouyancyForceGenerator = nullptr;

	// Base experimento Bolas lokas
	PxRigidStatic* baseEstatica = nullptr;
	PxShape* baseEstaticaShape = nullptr;
	RenderItem* baseEstaticaRenderItem = nullptr;

	// Mesa experimento quimica
	RenderItem* boteRenderItem = NULL, * boteRenderItem2 = NULL, * boteRenderItem3 = NULL, * mesaRenderItem = NULL, * mesaRenderItem2 = NULL, * pataRenderItem1 = NULL, * pataRenderItem2 = NULL, * pataRenderItem3 = NULL, * pataRenderItem4 = NULL, * pataRenderItem5 = NULL, * pataRenderItem6 = NULL;
	PxTransform bote, bote2, bote3, mesa, mesa2, pata1, pata2, pata3, pata4, pata5, pata6;

	// Pelota y barco
	Particle* pelota = nullptr; // Experimento pelota de aire
	RenderItem* baseRenderItem = NULL, * tapaRenderItem = NULL, * aireRenderItem = NULL;
	PxTransform base, tapa, aire;


public:
	Game(physx::PxPhysics* physics, physx::PxScene* scene);
	void nextState();
	void previousState();
	void update(double t);
	void keyPressed(unsigned char key);
	~Game();

protected:
	void createCircleOfParticles(Vector3 centerPosition);
	void updateElements(double t);

	// Start y End
	void startInit();
	void endInit();

	// Experimentos
	void experimentoColumnaAireInit();
	void experimentoFlotacionInit();
	void experimentoBolasLokasInit();
	void experimentoExplosionMotorInit();
	void experimentoQuimicaInit();
	void experimentoGrifoInfinitoInit();

	// Proyectiles
	void proyectilBala();
	void proyectilNieve();
	void proyectilBola();

	// Elimina
	void deleteAll();
	void deleteForces();
	void deleteSpecificForces();
	void deleteSolidos();
	void deleteTexts();
};

