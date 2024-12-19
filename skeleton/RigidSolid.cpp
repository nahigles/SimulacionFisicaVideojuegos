#include "RigidSolid.h"
#include "ForceGenerator.h"

RigidSolid::RigidSolid(physx::PxPhysics* physics, physx::PxScene* scene, float m, Vector3 materialInfo, Vector3 pos, Vector4 color, float lifeTime, float size, ShapeRS s)
{
	// Fisikas
	gPhysics = physics;
	gScene = scene;

	pose = physx::PxTransform(pos);

	this->vel = vel;
	this->acel = acel;
	this->forceAcum = acel;

	masa = m;
	this->size = size;
	this->density = masa / getVolume(); // Formula densidad d = m/v
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
	rigidbody->setMass(masa);


	actor = rigidbody;
	gScene->addActor(*rigidbody);
	rigidbody->attachShape(*shape);
	renderItem = new RenderItem(shape, rigidbody, color);

	// Distribucion de masas y momentos de inercia
	if (s == SPHERE_RS)
		PxRigidBodyExt::updateMassAndInertia(*rigidbody, density);
	else if (s == BOX_RS) {
		float iH = (1 / 12) * masa * (size * size + size * size); // Alto
		float iW = (1 / 12) * masa * (size * size + size * size); // Ancho
		float iD = (1 / 12) * masa * (size * size + size * size); // Profundo
		rigidbody->setMassSpaceInertiaTensor({ iH,iW,iD });
	}

	masa = rigidbody->getMass();
}

RigidSolid::~RigidSolid()
{
	if (renderItem != nullptr) {
		//renderItem->release();
		DeregisterRenderItem(renderItem);
		renderItem = nullptr;
	}
	if (shape != nullptr) {
		shape->release();
		shape = nullptr;
	}
	if (rigidbody != nullptr) {
		static_cast<physx::PxRigidDynamic*>(rigidbody)->release();
		rigidbody = nullptr;
	}
}

void RigidSolid::update(double t)
{
	updateForces(t);

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

void RigidSolid::setInertia(Vector3 i)
{

}

void RigidSolid::addForce(Vector3 f)
{
	rigidbody->addForce(f);
}

void RigidSolid::addForceGenerator(ForceGenerator* force)
{
	forces.push_back(force);
}

void RigidSolid::setMass(float m)
{
	masa = m;
	density = masa / (getVolume());;
	physx::PxRigidBodyExt::updateMassAndInertia(*rigidbody, physx::PxReal(density)); // (Densidad)
}

void RigidSolid::updateForces(double t)
{
	for (auto g : forces) {
		g->update(this, t);
	}
}
