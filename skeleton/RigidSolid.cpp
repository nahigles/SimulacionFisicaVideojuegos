#include "RigidSolid.h"

RigidSolid::RigidSolid(physx::PxPhysics* physics, physx::PxScene* scene, Vector3 materialInfo, Vector3 pos, Vector4 color, float lifeTime, float size, ShapeRS s)
{
	// Fisikas
	gPhysics = physics;
	gScene = scene;

	pose = physx::PxTransform(pos);
	physx::PxShape* shape;

	this->vel = vel;
	this->acel = acel;
	this->forceAcum = acel;
	
	this->size = size;
	//masa = m;
	alive = true;
	this->lifeTime = lifeTime;

	physx::PxSphereGeometry sphere(size);

	// Material fisico
	if (materialInfo != Vector3(-1, -1, -1)) {
		physx::PxMaterial* mMaterial;
		mMaterial = gPhysics->createMaterial(materialInfo.x, materialInfo.y, materialInfo.z);
		shape = CreateShape(sphere, mMaterial);
	}

	//shape = CreateShape(sphere);
	rigidbody = physics->createRigidDynamic(pose);
	actor = rigidbody;
	scene->addActor(*rigidbody);
	rigidbody->attachShape(*shape);
	renderItem = new RenderItem(shape, rigidbody, color);
}

void RigidSolid::update(double t)
{
	timer += t;
}

bool RigidSolid::isAlive()
{
	if (pose.p.y <= 0 || timer >= lifeTime)
		alive = false;

	return alive;
}

void RigidSolid::setVelocity(Vector3 v)
{
	rigidbody->setLinearVelocity(v);
}

void RigidSolid::setPosition(Vector3 p)
{
	auto aux = actor->getGlobalPose();
	aux.p = p;
	actor->setGlobalPose(aux);
}
