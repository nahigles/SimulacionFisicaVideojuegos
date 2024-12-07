#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"


enum ShapeRS {
	BOX_RS,
	SPHERE_RS,
	NONE_RS
};

class RigidSolid
{
protected:
	// Fisikas
	physx::PxPhysics* gPhysics = nullptr;
	physx::PxScene* gScene = nullptr;

	Vector3 vel;
	Vector3 acel;
	Vector3 forceAcum;
	physx::PxTransform pose;
	RenderItem* renderItem;
	double d = 0.987; // dumping (Entre 0 y 1)
	bool alive;
	double lifeTime;
	float timer = 0;
	double masa;
	double size;
	ShapeRS s;


	physx::PxRigidActor* actor = nullptr;
	physx::PxRigidBody* rigidbody = nullptr;


public:
	RigidSolid(physx::PxPhysics* physics = nullptr, physx::PxScene* scene = nullptr, Vector3 materialInfo = Vector3(-1, -1, -1), Vector3 pos = { 1.0f, 1.0f,1.0f }, Vector4 color = { 1.0f, 1.0f,1.0f,1.0f },float lifeTime = 30.0f, float size = 1, ShapeRS s = ShapeRS::SPHERE_RS);
	~RigidSolid() {}; // Tengo que borrar los punteros que creo con new. NO OLVIDAR! 
	void update(double t);
	bool isAlive();
	void setVelocity(Vector3 v);
	void setPosition(Vector3 p);
};

