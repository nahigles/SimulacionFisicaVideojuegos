#include "RigidSolidSystem.h"

RigidSolidSystem::RigidSolidSystem(physx::PxPhysics* physics, physx::PxScene* scene, Vector3 pos, Vector3 vel, double timeBetween, Vector3 positionVariation, Vector3 velocityVariation, RS_Type t, Vector4 color, bool direction)
{
	numElements = 0;
	timeCont = 0;
	initialPos = pos;
	initialVel = vel;
	this->timeBetween = timeBetween;
	randomPos = positionVariation;
	randomVel = velocityVariation;
	dir = direction;
	gPhysics = physics;
	gScene = scene;
	type = t;
	this->color = color;
	indexColor = 0;
}

void RigidSolidSystem::update(double t)
{
	// Actualizo particulas
	std::list<RigidSolid*>::iterator it = solidoRigidos.begin();
	while (it != solidoRigidos.end()) {

		(*it)->update(t);
		if (!(*it)->isAlive()) {
			delete (*it);
			it = solidoRigidos.erase(it);
			numElements--;
		}
		else
			++it;
	}

	// Creacion de particulas
	if (timeCont >= timeBetween && numElements <= maxNumElements) {
		createRigidSolid();
		timeCont = 0;
	}
	else
		timeCont += t;
}

RigidSolidSystem::~RigidSolidSystem()
{
	for (std::list<RigidSolid*>::iterator it = solidoRigidos.begin(); it != solidoRigidos.end(); ++it)
		delete (*it);
	solidoRigidos.clear();
}

void RigidSolidSystem::addForceGenerator(ForceGenerator* fGenerator)
{
	forceGenerators.push_back(fGenerator);
}

void RigidSolidSystem::createRigidSolid()
{
	// Distribucion uniforme
	std::random_device rd;
	std::mt19937 gen(rd());

	Vector3 aux;

	if (dir)
		aux = Vector3(0, 0, 0);
	else
		aux = -randomPos;

	// Masa aleatoria
	std::uniform_int_distribution<> distrMass(1, maxMass);

	// Random position distribution
	std::uniform_int_distribution<> distrPosX(aux.x, randomPos.x);
	std::uniform_int_distribution<> distrPosY(aux.y, randomPos.y);
	std::uniform_int_distribution<> distrPosZ(aux.z, randomPos.z);

	if (!dir)
		aux = -randomVel;

	// Random velocity distribution
	std::uniform_int_distribution<> distrVelX(aux.x, randomVel.x);
	std::uniform_int_distribution<> distrVelY(aux.y, randomVel.y);
	std::uniform_int_distribution<> distrVelZ(aux.z, randomVel.z);

	// Posicion aleatoria
	Vector3 newPosition = initialPos + Vector3(distrPosX(gen), distrPosY(gen), distrPosZ(gen));

	// Velocidad aleatoria
	Vector3 nerwVelocity = initialVel + Vector3(distrVelX(gen), distrVelY(gen), distrVelZ(gen));

	// Color
	if (type == COLOR) {
		color = colors.at(indexColor);
		indexColor = (indexColor + 1) % colors.size();
	}


	// RIGID SOLID
	RigidSolid* rS = new RigidSolid(gPhysics, gScene, distrMass(gen), {0.5,0.5,0.5}, newPosition, color, 20, 2);
	rS->setVelocity(nerwVelocity);

	// Añado generadores de fuerzas a los solidos rigidos
	for (auto it = forceGenerators.begin(); it != forceGenerators.end(); ++it) {
		rS->addForceGenerator(*it);
	}

	solidoRigidos.push_back(rS);

	numElements++;
}
