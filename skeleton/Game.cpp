#include "Game.h"
#include "Vector3D.h"

Game::Game()
{
	_state = START;
	_nextState = START;

	// Gravedad
	gravityForceGenerator = new GravityForceGenerator(10.0f);
	gravityForceGenerator2 = new GravityForceGenerator(-10.0f);
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
		case START: {
			std::cout << "Start State" << endl;

			// Gravedad
			gravityForceGenerator = new GravityForceGenerator(10.0f);
			gravityForceGenerator2 = new GravityForceGenerator(-10.0f);
			break;
		}
		case GAME: {
			std::cout << "Game State" << endl;

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
			particulas.push_back(new Particle(Vector3(0.0, 0.0, 0.0), Vector3(1.0, 0.0, 0.0), Vector3(1.0, 0.0, 0.0), 0.98, { 1.0, 0.0, 1.0, 1.0 }, 1.0, 3));
			break;
		}
		case FORCES: {
			std::cout << "Forces State" << endl;
			// Gravedad con
			// Box shape
			//gravityForceGenerator3 = new GravityForceGenerator(15.0f, { -20.0f, 10.0f, -20.0f }, { -15.0f, 60.0f, 20.0f });

			// Sphere shape
			gravityForceGenerator3 = new GravityForceGenerator(15.0f, { 0.0f, 60.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 30, false);
			windForceGenerator = new WindForceGenerator({ 0.0f, 60.0f, 0.0f }, 0.99/*, 0, { 5.0f, 30.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, 30*/);
			tornadoForceGenerator = new TornadoForceGenerator(0.99, 50, { 5.0f, 30.0f, 5.0f }/*, 35*/);
			blastForceGenerator = new BlastForceGenerator(500, 60, { 0.0f,50.0f,0.0f });

			break;
		}
		case END: {

			std::cout << "End State" << endl;
			deleteAll();
			deleteForces();

			break;
		}
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

	case FORCES:
	case GAME: {

		std::list<Particle*>::iterator itP = particulas.begin();
		while (itP != particulas.end()) {

			(*itP)->update(t);
			if (!(*itP)->isAlive()) {
				delete (*itP);
				itP = particulas.erase(itP);
			}
			else
				++itP;
		}

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
	case END: {

		break;
	}
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
		if (_state == GAME) {

			// LANZADOR DE BOLAS
			ParticleSystem* pSystem = new ParticleSystem({ 0.0,10.0,0.0 }, { 0.0, 8.0, 0.0 }, { 0.0, 5.0, 0.0 }, 2, { 0.0, 1.0, 0.56, 1.0 }, 7.0, 0.5, 5, 15, BALLS);
			particleSystems.push_back(pSystem);
			pSystem->addForceGenerator(gravityForceGenerator);
		}
		break;
	}
	case 'T': {
		if (_state == GAME) {

			// LLUVIA
			ParticleSystem* pSystem = new ParticleSystem({ 0.0,50.0,0.0 }, { 0.0, -10.0, 0.0 }, { 0.0, 20.0, 0.0 }, 0.25, { 0.0, 0.56, 0.9, 0.3 }, 9.0, 0.05, 20, 5, RAIN);
			particleSystems.push_back(pSystem);
			pSystem->addForceGenerator(gravityForceGenerator);
		}

		break;
	}
	case 'E': {
		if (_state == GAME) {
			// NIEVE
			ParticleSystem* pSystem = new ParticleSystem({ 0.0,50.0,0.0 }, { 0.0, -10.0, 0.0 }, { 0.0, 0.5, 0.0 }, 0.5, { 1.0, 1.0, 1.0, 0.0 }, 9.0, 0.05, 20, 5, SNOW);
			particleSystems.push_back(pSystem);
			pSystem->addForceGenerator(gravityForceGenerator);
		}
		break;
	}
	case 'R':
	{
		if (_state == GAME) {
			// ESPUMA
			ParticleSystem* pSystem = new ParticleSystem({ 0.0,50.0,0.0 }, { 10.0, 10.0, 0.0 }, { 0.0, 10.0, 0.0 }, 4, { 1.0, 1.0, 1.0, 1.0 }, 9.0, 0.05, 5, 15, FOAM);
			particleSystems.push_back(pSystem);
			pSystem->addForceGenerator(gravityForceGenerator);
		}
		break;
	}
	case 'Y':
	{
		if (_state == GAME) {
			// BOLAS COLORIDAS
			ParticleSystem* pSystem = new ParticleSystem({ 0.0,50.0,0.0 }, { 10.0, 10.0, 0.0 }, { 0.0, 10.0, 0.0 }, 3, { 1.0, 1.0, 1.0, 1.0 }, 9.0, 0.05, 5, 15, COLOURFULL);
			particleSystems.push_back(pSystem);
			pSystem->addForceGenerator(gravityForceGenerator);
		}
		break;
	}
	// GRAVITY TESTER
	case 'G':
	{
		if (_state == FORCES) {

			// Gravedad normal
			// Masa 1
			Particle* p = new Particle(Vector3(-10.0, 50.0, 10.0), Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 0.0), 0.98, { 0.2, 1.0, 1.0, 1.0 }, 4.0, 1);
			p->addForceGenerator(gravityForceGenerator);
			particulas.push_back(p);
			// Masa 4
			Particle* p2 = new Particle(Vector3(10.0, 50.0, -10.0), Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 0.0), 0.98, { 1.0, 1.0, 0.0, 1.0 }, 4.0, 4);
			p2->addForceGenerator(gravityForceGenerator);
			particulas.push_back(p2);

			// Gravedad al reves
			// Masa 1
			Particle* p3 = new Particle(Vector3(-10.0, 60.0, 10.0), Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 0.0), 0.98, { 0.2, 1.0, 1.0, 1.0 }, 4.0, 1);
			p3->addForceGenerator(gravityForceGenerator2);
			particulas.push_back(p3);
			// Masa 4
			Particle* p4 = new Particle(Vector3(10.0, 60.0, -10.0), Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 0.0), 0.98, { 1.0, 1.0, 0.0, 1.0 }, 4.0, 4);
			p4->addForceGenerator(gravityForceGenerator2);
			particulas.push_back(p4);
		}
		break;
	}
	// Invierte gravedad
	case 'F':
	{
		if (_state == FORCES)
			gravityForceGenerator2->changeGravity(-gravityForceGenerator2->getGravity());
		break;
	}
	case 'H':
	{
		if (_state == FORCES) {
			// Coloco muchas particulas y pongo un bounding box pequeño para ver que funciona
			int m = 1;
			for (int i = -50; i < 50; i += 10) {
				Particle* p = new Particle(Vector3(i, 60.0, -i), Vector3(10.0, 0.0, -10.0), Vector3(0.0, 0.0, 0.0), 0.98, { 1.0, 1.0, 0.0, 1.0 }, 2.0, m);
				p->addForceGenerator(gravityForceGenerator3);
				particulas.push_back(p);
				m++;
			}
		}
		break;
	}
	case 'V':
	{
		if (_state == FORCES) {
			// Vientoooo
			ParticleSystem* pSystem = new ParticleSystem({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 3, { 0,0,0, 0 }, 10, 0.5, 10, 0, COLOURFULL);
			particleSystems.push_back(pSystem);
			pSystem->addForceGenerator(windForceGenerator);
		}
		break;
	}
	case 'B':
	{
		if (_state == FORCES) {
			// Tornadoo
			ParticleSystem* pSystem = new ParticleSystem({ 30.0f, 30.0f, 9.0f }, { 20.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 7, { 0.0f, 0.0f, 0.0f,  0.0f }, 10, 0.05, 10, 10, COLOURFULL);
			particleSystems.push_back(pSystem);
			pSystem->addForceGenerator(tornadoForceGenerator);
		}
		break;
	}
	case 'C':
	{
		if (_state == FORCES) {

			// Creo cfigurao de particulas
			// Añado generador de fuerzas de explosion
			this->createCircleOfParticles({ 0.0f,50.0f,0.0f });
		}
		break;
	}

	case 'X': {
		if (_state == FORCES) {

			Particle* p = new Particle({ 0.0f,50.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, 0.98, { 0.0,0.0,0.0,1.0 }, 0.2, 2);
			particulas.push_back(p);
			blastForceGenerator->activateBlast();
		}
		break;
	}
	case 'Z': {
		// 2 particulas
		Particle* p1 = new Particle({ -10.0, 10.0,0.0 }, { 0.0, 0.0,0.0 }, { 0.0, 0.0,0.0 }, 0.85, { 0.0, 0.0,0.0, 1.0 }, 3, 1);
		Particle* p2 = new Particle({ 10.0, 10.0,0.0 }, { 0.0, 0.0,0.0 }, { 0.0, 0.0,0.0 }, 0.85, { 1.0, 1.0,1.0, 1.0 }, 3, 2);

		// Creo generadores de fuerza
		springForceGenerator1 = new SpringForceGenerator(1, 10, p2);
		springForceGenerator2 = new SpringForceGenerator(1, 10, p1);

		// Añado generadores de fuerza a las particulas
		p1->addForceGenerator(springForceGenerator1);
		p2->addForceGenerator(springForceGenerator2);

		// Añado particulas a la lista de particulas
		particulas.push_back(p1);
		particulas.push_back(p2);

		break;
	}
	case 'M':
	{
		deleteAll();
		break;
	}
	case 'P':
	{
		if (_state == GAME) {
			// Practica 1.2 [PROYECTILES]
			// Bola
			Camera* cam = GetCamera();
			Vector3 cPos = cam->getEye();
			Vector3 cDir = cam->getDir();

			float speed = 25.0f;
			Projectile* p = new Projectile(cPos, cDir * speed, Vector3(0.0, -4.5, 0.0), 0.98, { 1.0, 1.0, 0.0, 1.0 }, 2, 1.0f);
			p->addForceGenerator(gravityForceGenerator);
			projectiles.push_back(p);
		}
		break;
	}
	case 'I':
	{
		if (_state == GAME) {
			// Bala
			Camera* cam = GetCamera();
			Vector3 cPos = cam->getEye();
			Vector3 cDir = cam->getDir();

			float speed = 50.0f;
			Projectile* p = new Projectile(cPos, cDir * speed, Vector3(0.0, -4.5, 0.0), 0.98, { 0.0, 1.0, 1.0, 1.0 }, 3, 1.0f);
			p->addForceGenerator(gravityForceGenerator);
			projectiles.push_back(p);
		}
		break;
	}
	case 'O':
	{
		if (_state == GAME) {
			// Nieve
			Camera* cam = GetCamera();
			Vector3 cPos = cam->getEye();
			Vector3 cDir = cam->getDir();

			float speed = 7.0f;
			Projectile* p = new Projectile(cPos, cDir * speed, Vector3(0.0, -1.5, 0.0), 0.98, { 1.0, 1.0, 1.0, 1.0 }, 3, 1.0f);
			p->addForceGenerator(gravityForceGenerator);
			projectiles.push_back(p);
		}
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
	deleteForces();
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

	for (std::list<Particle*>::iterator it = particulas.begin(); it != particulas.end();) {
		delete (*it);
		it = particulas.erase(it);
	}

	for (std::list<ParticleSystem*>::iterator it = particleSystems.begin(); it != particleSystems.end();) {
		delete (*it);
		it = particleSystems.erase(it);
	}
}

void Game::deleteForces()
{
	// Borra Generadores de Gravedad
	if (gravityForceGenerator != nullptr) {
		delete gravityForceGenerator;
		gravityForceGenerator = nullptr;
	}
	if (gravityForceGenerator2 != nullptr) {
		delete gravityForceGenerator2;
		gravityForceGenerator2 = nullptr;
	}
	if (gravityForceGenerator3 != nullptr) {
		delete gravityForceGenerator3;
		gravityForceGenerator3 = nullptr;
	}

	if (windForceGenerator != nullptr) {
		delete windForceGenerator;
		windForceGenerator = nullptr;
	}

	if (tornadoForceGenerator != nullptr) {
		delete tornadoForceGenerator;
		tornadoForceGenerator = nullptr;
	}

	if (blastForceGenerator != nullptr) {
		delete blastForceGenerator;
		blastForceGenerator = nullptr;
	}

	if (springForceGenerator1 != nullptr) {
		delete springForceGenerator1;
		springForceGenerator1 = nullptr;
	}

	if (springForceGenerator2 != nullptr) {
		delete springForceGenerator2;
		springForceGenerator2 = nullptr;
	}
	
}

void Game::createCircleOfParticles(Vector3 centerPosition)
{

	// En cada particula añado el generador de fuerzas de la explosion
	int masa = 1;
	for (int i = -25; i <= 25; i += 10) {

		Particle* p1 = new Particle(Vector3(centerPosition.x + i, centerPosition.y, centerPosition.z), { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, 0.98, { 1.0,0.0,0.0,1.0 }, 1, masa);
		Particle* p2 = new Particle(Vector3(centerPosition.x, centerPosition.y + i, centerPosition.z), { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, 0.98, { 0.0,1.0,0.0,1.0 }, 1.5, masa + 1);
		Particle* p3 = new Particle(Vector3(centerPosition.x, centerPosition.y, centerPosition.z + i), { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, 0.98, { 0.0,0.0,1.0,1.0 }, 2, masa + 2);

		p1->addForceGenerator(blastForceGenerator);
		p2->addForceGenerator(blastForceGenerator);
		p3->addForceGenerator(blastForceGenerator);

		particulas.push_back(p1);
		particulas.push_back(p2);
		particulas.push_back(p3);

		masa++;
	}
}
