#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>

using namespace physx;

class ForceGenerator;

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
	float density = 0.15;
	double size;

	// Fuerzas
	std::list<ForceGenerator*> forces;

	// Forma
	ShapeRS s;
	physx::PxShape* shape = nullptr;

	physx::PxRigidActor* actor = nullptr;
	physx::PxRigidBody* rigidbody = nullptr;

public:
	RigidSolid(physx::PxPhysics* physics, physx::PxScene* scene, float density, Vector3 materialInfo = Vector3(-1, -1, -1), Vector3 pos = { 1.0f, 1.0f,1.0f }, Vector4 color = { 1.0f, 1.0f,1.0f,1.0f },float lifeTime = 30.0f, float size = 1, ShapeRS s = ShapeRS::SPHERE_RS);
	~RigidSolid();
	void update(double t);
	bool isAlive();
	void setVelocity(Vector3 v);
	void setPosition(Vector3 p);
	void setInertia(Vector3 i);
	inline Vector3 getVelocity() { return rigidbody->getLinearVelocity(); }
	inline Vector3 getPosition() { return actor->getGlobalPose().p; }
	inline double getMass() { return rigidbody->getMass(); }
	void addForce(Vector3 f);
	void addForceGenerator(ForceGenerator* force);
	inline double getHeight() { return size * 2; } // Alto
	inline double getVolume() { return size * 2 * size * 2 * size * 2; } // Ancho * Largo * Alto (En este caso es un cubo asiq son iguales)

protected:

	void updateForces(double t);
};

