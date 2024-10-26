#pragma once
#include "core.hpp"
#include <list>
#include "Particle.h"
#include "RenderUtils.hpp"
#include <random>
#include <cmath>
#include <iomanip>
#include <map>
#include <string>

enum ParticleType {
	RAIN,
	BALLS,
	SNOW,
	FOAM,
	COLOURFULL,
	LAST_ELEM
};

class ParticleSystem
{

protected:
	double randMinVel = -15;
	double randMaxVel = 15;
	double randMinPos = -5;
	double randMaxPos = 5;
	double timeBetweenParticles;
	ParticleType type;
	int maxNumParticle;
	int nParticles;

	// Atributos
	Vector3 initialpos;
	Vector3 initialVel;
	Vector3 initialAcel;
	float initialSize;
	Vector4 initialColor;
	//physx::PxGeometry initialShape;
	int initialLifeTime;

	// Lista para guardar particulas
	std::list<Particle*> particles;


	// Variables auxiliares
	double timeCont; // Contador del tiempo que ha pasado

	// Colores auxiliares
	int indexColor;
	std::vector<Vector4> colors = { {1.0,1.0,1.0,1.0},
	{0.0,0.0,0.0,1.0}, 
	{1.0,0.0,0.0,1.0}, 
	{0.0,1.0,0.0,1.0}, 
	{0.0,0.0,1.0,1.0},
	{1.0,1.0,0.0,1.0}, 
	{0.0,1.0,1.0,1.0}, 
	{1.0,0.0,1.0,1.0} };


	// METODOS
	void createParticle();

public:
	// Contructora
	ParticleSystem(Vector3 pos, Vector3 vel,Vector3 acel, float size, Vector4 color, double lifeTime, double timeBetween, double randomPos, double randomVel, ParticleType pType, int maxParticles = 3000);
	~ParticleSystem();
	void update(double t);

};

