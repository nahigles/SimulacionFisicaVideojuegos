#pragma once
#include "core.hpp"
#include "RigidSolid.h"
#include <list>
class SolidRigidSystem
{
protected:
	const int maxNumElements = 100;
	int numElements;

	Vector3 initialPos;
	Vector3 initialVel;
	double timeBetween;
	double timeCont;

	std::list<RigidSolid*> solidoRigidos;

	physx::PxPhysics* gPhysics = nullptr;
	physx::PxScene* gScene = nullptr;

public:
	SolidRigidSystem(physx::PxPhysics* physics, physx::PxScene* scene,Vector3 pos, Vector3 vel, double timeBetween);
	void update(double t);
	~SolidRigidSystem();

protected:
	void createElement();
};

