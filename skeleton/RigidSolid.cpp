#include "RigidSolid.h"

RigidSolid::RigidSolid(physx::PxPhysics* physics, physx::PxScene* scene, Vector3 materialInfo, Vector3 pos, Vector4 color, float size, ShapeRS s)
{
	// Fisikas
	gPhysics = physics;
	gScene = scene;

	pose = physx::PxTransform(pos);
	physx::PxShape* shape;

	this->vel = vel;
	this->acel = acel;
	this->forceAcum = acel;
	
	//this->renderItem;
	//this->d = damping; // dumping (Entre 0 y 1)
	this->size = size;
	//masa = m;
	alive = true;
	lifeTime = 0;

	physx::PxSphereGeometry sphere(size);

	// Material fisico
	if (materialInfo != Vector3(-1, -1, -1)) {
		physx::PxMaterial* mMaterial;
		mMaterial = gPhysics->createMaterial(materialInfo.x, materialInfo.y, materialInfo.z);
		shape = CreateShape(sphere, mMaterial);
	}

	//shape = CreateShape(sphere);
	body = physics->createRigidDynamic(pose);
	actor = body;
	scene->addActor(*body);
	body->attachShape(*shape);
	renderItem = new RenderItem(shape, body, color);
}
