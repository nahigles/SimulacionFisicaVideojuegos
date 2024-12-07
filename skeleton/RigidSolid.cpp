#include "RigidSolid.h"

RigidSolid::RigidSolid(physx::PxPhysics* physics, physx::PxScene* scene, Vector3 materialInfo, Vector3 pos, Vector4 color, float lifeTime, float size, ShapeRS s)
{
	// Fisikas
	gPhysics = physics;
	gScene = scene;

	pose = physx::PxTransform(pos);

	this->vel = vel;
	this->acel = acel;
	this->forceAcum = acel;

	this->size = size;
	//masa = m;
	alive = true;
	this->lifeTime = lifeTime;

	physx::PxSphereGeometry sphere(size);
	physx::PxBoxGeometry box(size, size, size);


	// Material fisico
	if (materialInfo != Vector3(-1, -1, -1)) {
		physx::PxMaterial* mMaterial;
		mMaterial = gPhysics->createMaterial(materialInfo.x, materialInfo.y, materialInfo.z);

		// Crea forma
		if (s == SPHERE_RS)
			shape = CreateShape(sphere, mMaterial);
		else if (s == BOX_RS)
			shape = CreateShape(box, mMaterial);

	}

	rigidbody = physics->createRigidDynamic(pose);
	actor = rigidbody;
	scene->addActor(*rigidbody);
	rigidbody->attachShape(*shape);
	renderItem = new RenderItem(shape, rigidbody, color);
}

RigidSolid::~RigidSolid()
{
	if (renderItem != nullptr) {
		DeregisterRenderItem(renderItem);
		renderItem = nullptr;
	}
	shape->release();
	static_cast<physx::PxRigidDynamic*>(rigidbody)->release();
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
