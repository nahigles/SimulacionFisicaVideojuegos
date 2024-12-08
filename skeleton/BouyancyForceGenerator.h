#pragma once
#include "ForceGenerator.h"
class BouyancyForceGenerator : public ForceGenerator
{

protected:
    float _height;
    float _liquid_density;

    Particle* _liquid_particle;

public:

    BouyancyForceGenerator(float h, float d);

    void update(Particle* particle, double t) override;

    virtual void update(RigidSolid* rs, double t) override {};

    virtual ~BouyancyForceGenerator();
};

