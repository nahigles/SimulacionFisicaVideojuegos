#include "ForceGenerator.h"

ForceGenerator::ForceGenerator(Vector3 p1, Vector3 p2, float r, bool visible)
{
	Vector3 zeroVector = { 0.0f,0.0f,0.0f };

	if (p2 == zeroVector && r == 0)
		usingBounding = false;
	else {
		usingBounding = true;
		// SI radio no es 0 --> Hacemos esfera
		if (r) {
			boundingShape = new BoundingSphere(p1, r, visible);
		}
		// Sino caja
		else {
			boundingShape = new BoundingBox(p1, p2, visible);
		}
	}
}

ForceGenerator::~ForceGenerator()
{
	if (boundingShape != nullptr) {
		delete boundingShape;
		boundingShape = nullptr;
	}
}
