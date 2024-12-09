#pragma once
#include "core.hpp"
#include "RigidSolid.h"
#include <random>
#include <cmath>
#include <list>


enum RS_Type {
	BASIC,
	COLOR
};

class RigidSolidSystem
{
	

protected:
	const int maxNumElements = 100;
	int numElements;

	Vector3 initialPos;
	Vector3 initialVel;
	double timeBetween;
	double timeCont;

	// Variacion random de cada eje
	Vector3 randomVel; 
	Vector3 randomPos;
	bool dir;

	RS_Type type;

	// Colores
	Vector4 color;

	// Colores auxiliares
	int indexColor;
	std::vector<Vector4> colors = { {1.0,1.0,1.0,1.0},
	{0.0,0.0,0.0,1.0},
	{1.0,0.0,0.0,1.0},
	{0.0,1.0,0.0,1.0},
	{0.0,0.0,1.0,1.0},
	{1.0,1.0,0.0,1.0},
	{0.0,1.0,1.0,1.0},
	{1.0,0.0,1.0,1.0} };

	// Solidos Rigidos
	std::list<RigidSolid*> solidoRigidos;

	// Force Generators
	std::list<ForceGenerator*> forceGenerators;

	// physx
	physx::PxPhysics* gPhysics = nullptr;
	physx::PxScene* gScene = nullptr;

public:
	RigidSolidSystem(physx::PxPhysics* physics, physx::PxScene* scene, Vector3 pos, Vector3 vel, double timeBetween, Vector3 positionVariation = {0,0,0}, Vector3 velocityVariation = {0,0,0}, RS_Type t = BASIC, Vector4 color = { 1,1,1,1 }, bool direction = false);
	void update(double t);
	~RigidSolidSystem();
	void addForceGenerator(ForceGenerator* fGenerator);

protected:
	void createRigidSolid();
};

