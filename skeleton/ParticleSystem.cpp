#include "ParticleSystem.h"
#include <iostream>

void ParticleSystem::createParticle()
{
	// Distribucion uniforme
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(randMinVel, randMaxVel);
	std::uniform_int_distribution<> distrPos(randMinPos, randMaxPos);

	// Distribucion normal
	std::normal_distribution<> d{ 5.0, 10.0 };

	Vector3 posRandom;
	Vector3 randomVel;

	switch (type)
	{
	case RAIN:
		// Posicion aleatoria
		posRandom = Vector3(distrPos(gen), 0.0, distrPos(gen));
		posRandom = posRandom + initialpos;

		// Velocidad aleatoria
		randomVel = Vector3(distr(gen), distr(gen), distr(gen));
		randomVel = randomVel + initialVel;
		break;
	case BALLS: {

		// Posicion aleatoria
		posRandom = Vector3(distrPos(gen), distrPos(gen), distrPos(gen));
		posRandom = posRandom + initialpos;

		// Velocidad aleatoria
		randomVel = Vector3(distr(gen), distr(gen), distr(gen));
		randomVel = randomVel + initialVel;
		break;
	}
	case SNOW: {

		// Posicion aleatoria
		posRandom = Vector3(distrPos(gen), 0.0, distrPos(gen));
		posRandom = posRandom + initialpos;

		// Velocidad aleatoria
		randomVel = Vector3(distr(gen), distr(gen), distr(gen));
		randomVel = randomVel + initialVel;
		break;
	}
	case FOAM: {

		// Posicion aleatoria
		posRandom = Vector3(distrPos(gen), 0.0, distrPos(gen));
		posRandom = posRandom + initialpos;

		// Velocidad aleatoria
		randomVel = Vector3(0.0, distr(gen), 0.0);
		randomVel = randomVel + initialVel;
		break;
	}
	case COLOURFULL: {

		// Posicion aleatoria
		posRandom = Vector3(d(gen), d(gen), d(gen));
		posRandom = posRandom + initialpos;

		// Velocidad aleatoria
		randomVel = Vector3(d(gen), d(gen), d(gen));
		randomVel = randomVel + initialVel;
		initialColor = colors.at(indexColor);
		indexColor = (indexColor + 1) % colors.size();
		
		break;
	}
	default:
		break;
	}

	particles.push_back(new Particle(posRandom, randomVel, initialAcel, 0.98, initialColor, initialSize));
	nParticles++;
}

ParticleSystem::ParticleSystem(Vector3 pos, Vector3 vel, Vector3 acel, float size, Vector4 color, double lifeTime, double timeBetween, double randomPos, double randomVel, ParticleType pType, int maxParticles)
{
	initialpos = pos;
	initialVel = vel;
	initialAcel = acel;
	initialSize = size;
	initialColor = color;
	initialLifeTime = lifeTime;
	timeBetweenParticles = timeBetween;
	timeCont = timeBetween;
	randMaxPos = randomPos;
	randMinPos = -randomPos;
	randMaxVel = randomVel;
	randMinVel = -randomVel;
	type = pType;
	indexColor = 0;
	maxNumParticle = maxParticles;
	nParticles = 0;
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
			nParticles--;
		}
		else
			++it;
	}

	// Creacion de particulas
	if (timeCont >= timeBetweenParticles && nParticles <= maxNumParticle) {
		createParticle();
		timeCont = 0;
	}
	else
		timeCont += t;

}
