#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

enum Shape { BOX, SPHERE, NONE };

class Bounding
{

public:
	Bounding() {};
	virtual bool isInside(Vector3 point) = 0;
	virtual ~Bounding() {};

protected:
	RenderItem* shapeItemRender;
	physx::PxTransform shapeTransform;
};

