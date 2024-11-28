#pragma once
#include "ForceGenerator.h"
class BlastForceGenerator : public ForceGenerator
{
protected:
	float intensity;
	float radio;

public:
	BlastForceGenerator();

};

