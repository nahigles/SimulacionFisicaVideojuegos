#include "SolidRigidSystem.h"

SolidRigidSystem::SolidRigidSystem(physx::PxPhysics* physics, physx::PxScene* scene,Vector3 pos, Vector3 vel, double timeBetween)
{
	numElements = 0;
	timeCont = 0;
	initialPos = pos;
	initialVel = vel;
	this->timeBetween = timeBetween;

	gPhysics = physics;
	gScene = scene;
}

void SolidRigidSystem::update(double t)
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
		createElement();
		timeCont = 0;
	}
	else
		timeCont += t;
}

SolidRigidSystem::~SolidRigidSystem()
{
	for (std::list<RigidSolid*>::iterator it = solidoRigidos.begin(); it != solidoRigidos.end(); ++it)
		delete (*it);
	solidoRigidos.clear();
}

void SolidRigidSystem::createElement()
{
	RigidSolid* rS = new RigidSolid(gPhysics, gScene, { 0.5,0.5,0.5 }, { 0.0,70,0 }, { 1,1,1,1 }, 20, 2);
	rS->setVelocity({ 5,0,0 });
	solidoRigidos.push_back(rS);
}
