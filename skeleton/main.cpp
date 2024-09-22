#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include "Vector3D.h"

std::string display_text = "This is a test";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;


PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;

RenderItem* xRenderItem = NULL, * yRenderItem = NULL, * zRenderItem = NULL, *originRenderItem = NULL;
PxTransform x, y, z, origin;

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

	// Dibujo bola
	origin = PxTransform(originVector.x(), originVector.y(), originVector.z());
	x = PxTransform(xVector.x(), xVector.y(), xVector.z());
	y = PxTransform(yVector.x(), yVector.y(), yVector.z());
	z = PxTransform(zVector.x(), zVector.y(), zVector.z());
	originRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &origin, { 1.0, 1.0, 1.0, 1.0});
	xRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &x, { 1.0, 0.0, 0.0, 1.0});
	yRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &y, { 0.0,1.0, 0.0, 1.0});
	zRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &z, { 0.0, 0.0, 1.0, 1.0});
	
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);
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
	DeregisterRenderItem(originRenderItem);
	DeregisterRenderItem(xRenderItem);
	DeregisterRenderItem(yRenderItem);
	DeregisterRenderItem(zRenderItem);
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
		//case 'B': break;
		//case ' ':	break;
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