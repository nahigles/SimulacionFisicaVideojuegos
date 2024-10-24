#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>
#include <list>
#include <iostream>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include "Vector3D.h"
#include "Particle.h"
#include "Projectile.h"
#include "ParticleSystem.h"

std::string display_text = "This is a test";


using namespace physx;
using namespace std;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;


PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;

RenderItem* xRenderItem = NULL, * yRenderItem = NULL, * zRenderItem = NULL, * originRenderItem = NULL;
PxTransform x, y, z, origin;

Particle* particle;
list<Projectile*> projectiles;

list<ParticleSystem*> particleSystems; // SISTEMA DE PARTICULAS


// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	// Practica 0
	Vector3D originVector = Vector3D();
	Vector3D xVector = Vector3D(10, 0, 0);
	Vector3D yVector = Vector3D(0, 10, 0);
	Vector3D zVector = Vector3D(0, 0, 10);

	// Dibujo bolas
	origin = PxTransform(originVector.x(), originVector.y(), originVector.z());
	x = PxTransform(xVector.x(), xVector.y(), xVector.z());
	y = PxTransform(yVector.x(), yVector.y(), yVector.z());
	z = PxTransform(zVector.x(), zVector.y(), zVector.z());
	originRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &origin, { 1.0, 1.0, 1.0, 1.0 });
	xRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &x, { 1.0, 0.0, 0.0, 1.0 });
	yRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &y, { 0.0,1.0, 0.0, 1.0 });
	zRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &z, { 0.0, 0.0, 1.0, 1.0 });


	// Practica 1.1 [PARTICULAS]
	particle = new Particle(Vector3(0.0, 0.0, 0.0), Vector3(1.0, 0.0, 0.0), Vector3(1.0, 0.0, 0.0), 0.98, { 1.0, 0.0, 1.0, 1.0 }, 1.0);

}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);


	// Actualizo particula
	//particle->integrate(t);
	if (particle != nullptr)
		particle->update(t);

	std::list<Projectile*>::iterator it = projectiles.begin();
	while (it != projectiles.end()) {

		(*it)->update(t);
		if (!(*it)->isAlive()) {
			delete (*it);
			it = projectiles.erase(it);
		}
		else
			++it;
	}

	// Sistema de particulas
	std::list<ParticleSystem*>::iterator it2 = particleSystems.begin();
	while (it2 != particleSystems.end()) {

		(*it2)->update(t);
		++it2;
	}
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();

	// Importante Deregistear
	//DeregisterRenderItem(originRenderItem);
	//DeregisterRenderItem(xRenderItem);
	//DeregisterRenderItem(yRenderItem);
	//DeregisterRenderItem(zRenderItem);

	//for (std::list<Projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it)
	//	delete (*it);
	//projectiles.clear();

	//delete particle;

	//for (std::list<ParticleSystem*>::iterator it = particleSystems.begin(); it != particleSystems.end(); ++it)
	//	delete (*it);
	//particleSystems.clear();

			// Borra todo
	if (originRenderItem != nullptr) {
		DeregisterRenderItem(originRenderItem);
		originRenderItem = nullptr;
	}
	if (xRenderItem != nullptr) {
		DeregisterRenderItem(xRenderItem);
		xRenderItem = nullptr;
	}
	if (yRenderItem != nullptr) {
		DeregisterRenderItem(yRenderItem);
		yRenderItem = nullptr;
	}
	if (zRenderItem != nullptr) {
		DeregisterRenderItem(zRenderItem);
		zRenderItem = nullptr;
	}

	for (std::list<Projectile*>::iterator it = projectiles.begin(); it != projectiles.end();) {
		delete (*it);
		it = projectiles.erase(it);
	}
	projectiles.clear();

	if (particle != nullptr) {
		delete particle;
		particle = nullptr;
	}

	for (std::list<ParticleSystem*>::iterator it = particleSystems.begin(); it != particleSystems.end();) {
		delete (*it);
		it = particleSystems.erase(it);
	}
	particleSystems.clear();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
		//case 'B': break;
		//case ' ':	break;
	case 'Q':
	{
		// LANZADOR DE BOLAS
		particleSystems.push_back(new ParticleSystem({ 0.0,10.0,0.0 }, { 0.0, 8.0, 0.0 }, { 0.0, -5.0, 0.0 }, 2, { 0.0, 1.0, 0.56, 1.0 }, 7.0, 0.5, 5, 15, BALLS));
		break;
	}
	case 'T': {
		// LLUVIA
		particleSystems.push_back(new ParticleSystem({ 0.0,50.0,0.0 }, { 0.0, -10.0, 0.0 }, { 0.0, -20.0, 0.0 }, 0.25, { 0.0, 0.56, 0.9, 0.3 }, 9.0, 0.05, 20, 5, RAIN));

		break;
	}
	case 'E': {
		// NIEVE
		particleSystems.push_back(new ParticleSystem({ 0.0,50.0,0.0 }, { 0.0, -10.0, 0.0 }, { 0.0, -0.5, 0.0 }, 0.5, { 1.0, 1.0, 1.0, 0.0 }, 9.0, 0.05, 20, 5, SNOW));
		break;
	}
	case 'R':
	{
		// ESPUMA
		particleSystems.push_back(new ParticleSystem({ 0.0,50.0,0.0 }, { 10.0, 10.0, 0.0 }, { 0.0, -10.0, 0.0 }, 4, { 1.0, 1.0, 1.0, 1.0 }, 9.0, 0.05, 5, 15, FOAM));
		break;
	}
	case 'Y':
	{
		// BOLAS COLORIDAS
		particleSystems.push_back(new ParticleSystem({ 0.0,50.0,0.0 }, { 10.0, 10.0, 0.0 }, { 0.0, -10.0, 0.0 }, 3, { 1.0, 1.0, 1.0, 1.0 }, 9.0, 0.05, 5, 15, COLOURFULL));
		break;
	}
	case 'M':
	{
		// Borra todo
		if (originRenderItem != nullptr) {
			DeregisterRenderItem(originRenderItem);
			originRenderItem = nullptr;
		}
		if (xRenderItem != nullptr) {
			DeregisterRenderItem(xRenderItem);
			xRenderItem = nullptr;
		}
		if (yRenderItem != nullptr) {
			DeregisterRenderItem(yRenderItem);
			yRenderItem = nullptr;
		}
		if (zRenderItem != nullptr) {
			DeregisterRenderItem(zRenderItem);
			zRenderItem = nullptr;
		}

		for (std::list<Projectile*>::iterator it = projectiles.begin(); it != projectiles.end();) {
			delete (*it);
			it = projectiles.erase(it);
		}

		if (particle != nullptr) {
			delete particle;
			particle = nullptr;
		}

		for (std::list<ParticleSystem*>::iterator it = particleSystems.begin(); it != particleSystems.end();) {
			delete (*it);
			it = particleSystems.erase(it);
		}

		break;
	}
	case 'P':
	{

		// Practica 1.2 [PROYECTILES]
		// Bola
		Camera* cam = GetCamera();
		Vector3 cPos = cam->getEye();
		Vector3 cDir = cam->getDir();

		float speed = 25.0f;
		projectiles.push_back(new Projectile(cPos, cDir * speed, Vector3(0.0, -4.5, 0.0), 0.98, { 1.0, 1.0, 0.0, 1.0 }, 2, 1.0f));
		cout << "X: " << cDir.x << "Y: " << cDir.y << "Z: " << cDir.z << endl;
		break;
	}
	case 'B':
	{
		// Bala
		Camera* cam = GetCamera();
		Vector3 cPos = cam->getEye();
		Vector3 cDir = cam->getDir();

		float speed = 50.0f;
		projectiles.push_back(new Projectile(cPos, cDir * speed, Vector3(0.0, -4.5, 0.0), 0.98, { 0.0, 1.0, 1.0, 1.0 }, 3, 1.0f));
		cout << "X: " << cDir.x << "Y: " << cDir.y << "Z: " << cDir.z << endl;
		break;
	}
	case 'N':
	{
		// Nieve
		Camera* cam = GetCamera();
		Vector3 cPos = cam->getEye();
		Vector3 cDir = cam->getDir();

		float speed = 7.0f;
		projectiles.push_back(new Projectile(cPos, cDir * speed, Vector3(0.0, -1.5, 0.0), 0.98, { 1.0, 1.0, 1.0, 1.0 }, 3, 1.0f));
		cout << "X: " << cDir.x << "Y: " << cDir.y << "Z: " << cDir.z << endl;
		break;
	}
	case ' ':
	{
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char* const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for (PxU32 i = 0; i < frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}