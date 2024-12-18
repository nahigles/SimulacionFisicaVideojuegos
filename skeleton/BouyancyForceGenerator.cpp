#include "BouyancyForceGenerator.h"

BouyancyForceGenerator::BouyancyForceGenerator(float h, float d) : ForceGenerator(), _liquid_density(d) {
    _liquid_particle = new Particle({ 0,20,0 }, { 0,0,0 }, { 0,0,0 }, 0.60, { 0.0,0.3,1.0, 1 }, 10, 1, CUBO);
}

void BouyancyForceGenerator::update(Particle* particle, double t)
{
    float h = particle->getPos().y;
    float h0 = _liquid_particle->getPos().y;

    _height = particle->getHeight();

    Vector3 f(0, 0, 0);
    float immersed = 0.0;
    if (h - h0 > _height * 0.5)
        immersed = 0.0;
    else if (h0 - h > _height * 0.5)
        immersed = 1.0;
    else
        immersed = (h0 - h) / _height + 0.5;
    f.y = _liquid_density * particle->getVolume() * immersed * 9.8;

    particle->addForce(f);
}

void BouyancyForceGenerator::update(RigidSolid* rs, double t)
{
    float h = rs->getPosition().y;
    float h0 = _liquid_particle->getPos().y;

    _height = rs->getHeight();

    Vector3 f(0, 0, 0);
    float immersed = 0.0;
    if (h - h0 > _height * 0.5)
        immersed = 0.0;
    else if (h0 - h > _height * 0.5)
        immersed = 1.0;
    else
        immersed = (h0 - h) / _height + 0.5;
    f.y = _liquid_density * rs->getVolume() * immersed * 9.8;

    rs->addForce(f);
}

BouyancyForceGenerator::~BouyancyForceGenerator()
{
	delete _liquid_particle;
	_liquid_particle = nullptr;
}
