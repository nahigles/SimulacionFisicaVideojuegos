#include "ParticleSystem.h"
#include <iostream>

void ParticleSystem::createParticle()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(randMin, randMax);

	// Posicion aleatoria (tengo q hacerla aleatorria todavia)
	Vector3 posRandom = initialpos;

	// Velocidad aleatoria
	Vector3 randomVel = Vector3(distr(gen), distr(gen), distr(gen));
	randomVel = randomVel + initialVel;

	particles.push_back(new Particle(posRandom, randomVel, { 0.0,-10.0,0.0 }, 0.98, initialColor, initialSize));
}

ParticleSystem::ParticleSystem(Vector3 pos, Vector3 vel, int size, Vector4 color, double lifeTime, double timeBetween)
{
	initialpos = pos;
	initialVel = vel;
	initialSize = size;
	initialColor = color;
	initialLifeTime = lifeTime;
	timeBetweenParticles = timeBetween;
	timeCont = timeBetween;
}

ParticleSystem::~ParticleSystem()
{
	for (std::list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it)
		delete (*it);
	particles.clear();
}

void ParticleSystem::update(double t)
{
	// Actualizo particulas
	std::list<Particle*>::iterator it = particles.begin();
	while (it != particles.end()) {

		(*it)->update(t);
		if (!(*it)->isAlive() || (*it)->getLifeTime() >= initialLifeTime) {
			delete (*it);
			it = particles.erase(it);
		}
		else
			++it;
	}

	// Creacion de particulas
	if (timeCont >= timeBetweenParticles) {
		createParticle();
		timeCont = 0;
	}
	else
		timeCont += t;

}
