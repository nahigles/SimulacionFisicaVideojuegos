#include "Game.h"
#include "Vector3D.h"

Game::Game()
{
	_state = START;
	_nextState = START;
	gravityForceGenerator = new GravityForceGenerator(10.0f); // Pongo gravedad
}

void Game::nextState()
{
	_nextState = (State)((_nextState + 1) % LAST_STATE);
}

void Game::update(double t)
{
	if (_state != _nextState) {
		_state = _nextState;

		switch (_state)
		{
		case START:
			cout << "START" << endl;
			break;
		case GAME: {
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
			particle = new Particle(Vector3(0.0, 0.0, 0.0), Vector3(1.0, 0.0, 0.0), Vector3(1.0, 0.0, 0.0), 0.98, { 1.0, 0.0, 1.0, 1.0 }, 1.0, 3);
			break;
		}
		case END:
			deleteAll();
			break;
		case LAST_STATE:
			break;
		default:
			break;
		}
	}

	switch (_state)
	{
	case START:
		break;
	case GAME: {

		// Actualizo particula
		if (particle != nullptr)
			particle->update(t);
		if (particleG1 != nullptr)
			particleG1->update(t);
		if (particleG2 != nullptr)
			particleG2->update(t);

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

		break;
	}
	case END:
		break;
	case LAST_STATE:
		break;
	default:
		break;
	}
}

void Game::keyPressed(unsigned char key)
{
	switch (key)
	{
	case 'U':
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
		particleSystems.push_back(new ParticleSystem({ 0.0,50.0,0.0 }, { 10.0, 10.0, 0.0 }, { 0.0, 0.0, 0.0 }/*{ 0.0, -10.0, 0.0 }*/, 3, { 1.0, 1.0, 1.0, 1.0 }, 9.0, 0.05, 5, 15, COLOURFULL));
		break;
	}
	// GRAVITY TESTER
	case 'G':
	{
		particleG1 = new Particle(Vector3(-10.0, 50.0, 0.0), Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 0.0), 0.98, { 0.0, 1.0, 1.0, 1.0 }, 4.0, 1);
		particleG1->addForceGenerator(gravityForceGenerator);
		particleG2 = new Particle(Vector3(10.0, 50.0, 0.0), Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 0.0), 0.98, { 1.0, 1.0, 0.0, 1.0 }, 4.0, 4);
		particleG2->addForceGenerator(gravityForceGenerator);
		break;
	}
	case 'M':
	{
		deleteAll();
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
		Projectile* p = new Projectile(cPos, cDir * speed, Vector3(0.0, -4.5, 0.0), 0.98, { 1.0, 1.0, 0.0, 1.0 }, 2, 1.0f);
		p->addForceGenerator(gravityForceGenerator);
		projectiles.push_back(p);
		break;
	}
	case 'I':
	{
		// Bala
		Camera* cam = GetCamera();
		Vector3 cPos = cam->getEye();
		Vector3 cDir = cam->getDir();

		float speed = 50.0f;
		Projectile* p = new Projectile(cPos, cDir * speed, Vector3(0.0, -4.5, 0.0), 0.98, { 0.0, 1.0, 1.0, 1.0 }, 3, 1.0f);
		p->addForceGenerator(gravityForceGenerator);
		projectiles.push_back(p);
		break;
	}
	case 'O':
	{
		// Nieve
		Camera* cam = GetCamera();
		Vector3 cPos = cam->getEye();
		Vector3 cDir = cam->getDir();

		float speed = 7.0f;
		Projectile* p = new Projectile(cPos, cDir * speed, Vector3(0.0, -1.5, 0.0), 0.98, { 1.0, 1.0, 1.0, 1.0 }, 3, 1.0f);
		p->addForceGenerator(gravityForceGenerator);
		projectiles.push_back(p);
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

Game::~Game()
{
	deleteAll();

	// Borra Generador de Gravedad
	delete gravityForceGenerator;
	gravityForceGenerator = nullptr;
}

void Game::deleteAll()
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
	if (particleG1 != nullptr) {
		delete particleG1;
		particleG1 = nullptr;
	}
	if (particleG2 != nullptr) {
		delete particleG2;
		particleG2 = nullptr;
	}

	for (std::list<ParticleSystem*>::iterator it = particleSystems.begin(); it != particleSystems.end();) {
		delete (*it);
		it = particleSystems.erase(it);
	}



}
