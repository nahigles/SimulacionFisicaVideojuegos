#include "Game.h"
#include "Vector3D.h"

std::string display_text = " ";
std::string start_text = " ";
std::string end_text = " ";
std::string exp1_text = " ";
std::string exp2_text = " ";
std::string exp3_text = " ";
std::string exp4_text = " ";
std::string exp5_text = " ";
std::string exp6_text = " ";

Game::Game(physx::PxPhysics* physics, physx::PxScene* scene)
{
	gPhysics = physics;
	gScene = scene;

	_state = START;
	_nextState = START;

	// Gravedad
	gravityForceGenerator = new GravityForceGenerator(10.0f);
	gravityForceGenerator2 = new GravityForceGenerator(10.0f);
	gravityForceGenerator3 = new GravityForceGenerator(50.0f);

	startInit();
}

void Game::nextState()
{
	_nextState = (State)((_nextState + 1) % LAST_STATE);
}

void Game::previousState()
{
	if (_nextState != START)
		_nextState = (State)((_nextState - 1));
}

void Game::update(double t)
{
	// Se hace al cambiar de estado
	if (_state != _nextState) {
		_state = _nextState;

		_pause = false;

		switch (_state)
		{
		case START: {
			deleteAll();
			deleteSolidos();
			deleteSpecificForces();
			startInit();
			break;
		}
		case E_FLOTACION: {
			deleteAll();
			deleteSolidos();
			deleteSpecificForces();
			experimentoFlotacionInit();

			break;
		}
		case E_COLUMNA_AIRE: {
			deleteAll();
			deleteSolidos();
			deleteSpecificForces();
			experimentoColumnaAireInit();
			break;
		}
		case E_BOLAS_LOKAS: {
			deleteAll();
			deleteSolidos();
			deleteSpecificForces();
			experimentoBolasLokasInit();
			break;
		}
		case E_EXPLOSION_MOTOR:
		{
			deleteAll();
			deleteSolidos();
			deleteSpecificForces();
			experimentoExplosionMotorInit();
			break;
		}
		case E_QUIMICA: {
			deleteAll();
			deleteSolidos();
			deleteSpecificForces();
			experimentoQuimicaInit();
			break;
		}
		case E_GRIFO_INFINITO:
		{
			deleteAll();
			deleteSolidos();
			deleteSpecificForces();
			experimentoGrifoInfinitoInit();
			break;
		}
		case END: {
			deleteAll();
			deleteSolidos();
			deleteSpecificForces();
			tornadoForceGenerator = new TornadoForceGenerator(0.99, 50, { 5.0f, 30.0f, 5.0f }/*, 35*/);
			endInit();
			break;
		}
		case LAST_STATE:
			break;
		default:
			break;
		}
	}

	// Se hace todo el rato
	if (!_pause)
		updateElements(t);
}

void Game::keyPressed(unsigned char key)
{
	switch (key)
	{
	case 'P':
		// Pausa y despausa
		_pause = !_pause;
		break;

		// Proyectiles
	case 'U':
	{
		proyectilBola();
		break;
	}
	case 'I':
	{
		proyectilBala();
		break;
	}
	case 'Y':
	{
		proyectilNieve();
		break;
	}
	case 'F':
	{
		// Invierte gravedad
		if (_state == START)
			gravityForceGenerator2->changeGravity(-gravityForceGenerator2->getGravity());
		break;
	}
	case 'X': {
		// Activa explosion
		if (_state == E_EXPLOSION_MOTOR) {
			Particle* p = new Particle({ 0.0f,30.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, 0.98, { 0.0,0.0,0.0,1.0 }, 0.2, 2);
			particulas.push_back(p);
			blastForceGenerator->activateBlast();
		}

		break;
	}
	case 'C':
	{
		// Reinicia explosion
		if (_state == E_EXPLOSION_MOTOR) {
			deleteAll();
			deleteSolidos();
			deleteSpecificForces();
			experimentoExplosionMotorInit();
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
	if (pataRenderItem1 != nullptr) {
		DeregisterRenderItem(pataRenderItem1);
		pataRenderItem1 = nullptr;
	}
	if (pataRenderItem2 != nullptr) {
		DeregisterRenderItem(pataRenderItem2);
		pataRenderItem2 = nullptr;
	}

	if (pataRenderItem3 != nullptr) {
		DeregisterRenderItem(pataRenderItem3);
		pataRenderItem3 = nullptr;
	}

	if (pataRenderItem4 != nullptr) {
		DeregisterRenderItem(pataRenderItem4);
		pataRenderItem4 = nullptr;
	}

	if (pataRenderItem5 != nullptr) {
		DeregisterRenderItem(pataRenderItem5);
		pataRenderItem5 = nullptr;
	}

	if (baseRenderItem != nullptr) {
		DeregisterRenderItem(baseRenderItem);
		baseRenderItem = nullptr;
	}

	if (pataRenderItem6 != nullptr) {
		DeregisterRenderItem(pataRenderItem6);
		pataRenderItem6 = nullptr;
	}

	if (mesaRenderItem != nullptr) {
		DeregisterRenderItem(mesaRenderItem);
		mesaRenderItem = nullptr;
	}

	if (mesaRenderItem2 != nullptr) {
		DeregisterRenderItem(mesaRenderItem2);
		mesaRenderItem2 = nullptr;
	}

	if (boteRenderItem != nullptr) {
		DeregisterRenderItem(boteRenderItem);
		boteRenderItem = nullptr;
	}

	if (boteRenderItem2 != nullptr) {
		DeregisterRenderItem(boteRenderItem2);
		boteRenderItem2 = nullptr;
	}

	if (boteRenderItem3 != nullptr) {
		DeregisterRenderItem(boteRenderItem3);
		boteRenderItem3 = nullptr;
	}

	if (baseRenderItem != nullptr) {
		DeregisterRenderItem(baseRenderItem);
		baseRenderItem = nullptr;
	}

	if (tapaRenderItem != nullptr) {
		DeregisterRenderItem(tapaRenderItem);
		tapaRenderItem = nullptr;
	}

	if (aireRenderItem != nullptr) {
		DeregisterRenderItem(aireRenderItem);
		aireRenderItem = nullptr;
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

	for (std::list<RigidSolid*>::iterator it = rigidSolids.begin(); it != rigidSolids.end();) {
		delete (*it);
		it = rigidSolids.erase(it);
	}

	for (std::list<RigidSolidSystem*>::iterator it = rigidSolidSystems.begin(); it != rigidSolidSystems.end();) {
		delete (*it);
		it = rigidSolidSystems.erase(it);
	}

	deleteTexts();
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


	deleteSpecificForces();
}

void Game::deleteSpecificForces()
{
	if (windForceGenerator != nullptr) {
		delete windForceGenerator;
		windForceGenerator = nullptr;
	}

	if (windForceGeneratorPelota != nullptr) {
		delete windForceGeneratorPelota;
		windForceGeneratorPelota = nullptr;
	}

	if (windForceGeneratorBolas1 != nullptr) {
		delete windForceGeneratorBolas1;
		windForceGeneratorBolas1 = nullptr;
	}

	if (windForceGeneratorBolas2 != nullptr) {
		delete windForceGeneratorBolas2;
		windForceGeneratorBolas2 = nullptr;
	}

	if (windForceGeneratorBolas3 != nullptr) {
		delete windForceGeneratorBolas3;
		windForceGeneratorBolas3 = nullptr;
	}

	if (windForceGeneratorBolas4 != nullptr) {
		delete windForceGeneratorBolas4;
		windForceGeneratorBolas4 = nullptr;
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

	if (anchoredSpringForceGenerator != nullptr) {
		delete anchoredSpringForceGenerator;
		anchoredSpringForceGenerator = nullptr;
	}

	if (bouyancyForceGenerator != nullptr) {
		delete bouyancyForceGenerator;
		bouyancyForceGenerator = nullptr;
	}

}

void Game::deleteSolidos()
{
	// Borro suelo
	if (baseEstatica != nullptr) {
		baseEstatica->release();
		baseEstatica = nullptr;
	}
	if (baseEstaticaShape != nullptr) {
		baseEstaticaShape->release();
		baseEstaticaShape = nullptr;
	}
	if (baseEstaticaRenderItem != nullptr) {
		baseEstaticaRenderItem->release();
		baseEstaticaRenderItem = nullptr;
	}
}

void Game::deleteTexts()
{
	display_text = " ";
	start_text = " ";
	end_text = " ";
	exp1_text = " ";
	exp2_text = " ";
	exp3_text = " ";
	exp4_text = " ";
	exp5_text = " ";
	exp6_text = " ";
}

void Game::createCircleOfParticles(Vector3 centerPosition)
{

	// En cada particula añado el generador de fuerzas de la explosion
	int masa = 1;
	for (int i = -15; i <= 15; i += 5) {

		Particle* p1 = new Particle(Vector3(centerPosition.x + i, centerPosition.y, centerPosition.z), { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, 0.98, { 1.0,0.0,0.0,1.0 }, 0.5, masa);
		Particle* p2 = new Particle(Vector3(centerPosition.x, centerPosition.y + i, centerPosition.z), { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, 0.98, { 1.0,0.5,0.0,1.0 }, 1, masa + 1);
		Particle* p3 = new Particle(Vector3(centerPosition.x, centerPosition.y, centerPosition.z + i), { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, 0.98, { 1.0,1.0,0.0,1.0 }, 1.5, masa + 2);

		p1->addForceGenerator(blastForceGenerator);
		p2->addForceGenerator(blastForceGenerator);
		p3->addForceGenerator(blastForceGenerator);

		particulas.push_back(p1);
		particulas.push_back(p2);
		particulas.push_back(p3);

		masa++;
	}
}

void Game::updateElements(double t)
{
	// Particulas
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

	// Proyectiles
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

	// Update Rigid Solids
	std::list<RigidSolid*>::iterator itS = rigidSolids.begin();
	while (itS != rigidSolids.end()) {

		(*itS)->update(t);
		if (!(*itS)->isAlive()) {
			delete (*itS);
			itS = rigidSolids.erase(itS);
		}
		else
			++itS;
	}

	std::list<RigidSolidSystem*>::iterator itS2 = rigidSolidSystems.begin();
	while (itS2 != rigidSolidSystems.end()) {

		(*itS2)->update(t);
		++itS2;
	}
}

void Game::startInit()
{
	// BOLAS COLORIDAS
	ParticleSystem* pSystem2 = new ParticleSystem({ 0.0,50.0,0.0 }, { 10.0, 10.0, 0.0 }, { 0.0, 10.0, 0.0 }, 3, { 1.0, 1.0, 1.0, 1.0 }, 9.0, 0.01, 15, 15, COLOURFULL);
	particleSystems.push_back(pSystem2);
	pSystem2->addForceGenerator(gravityForceGenerator2);

	start_text = "- MAS ALLA DE LO VISIBLE -";
}

void Game::endInit()
{
	// Tornadoo
	ParticleSystem* pSystem2 = new ParticleSystem({ 25.0f, 30.0f, 4.0f }, { 20.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 7, { 0.0f, 0.0f, 0.0f,  0.0f }, 10, 0.05, 10, 10, COLOURFULL);
	particleSystems.push_back(pSystem2);
	pSystem2->addForceGenerator(tornadoForceGenerator);

	end_text = "Aqui termina esta simulacion. Espero que os haya gustado :)";
}

void Game::experimentoColumnaAireInit()
{
	// Maquina de aire
	// Vectores con posicion
	Vector3D baseVector = Vector3D(0, 25.5, 0);
	Vector3D tapaVector = Vector3D(0, 14, 0);
	Vector3D aireVector = Vector3D(0, 20, 0);

	// Transform
	base = PxTransform(baseVector.x(), baseVector.y(), baseVector.z());
	tapa = PxTransform(tapaVector.x(), tapaVector.y(), tapaVector.z());
	aire = PxTransform(aireVector.x(), aireVector.y(), aireVector.z());

	// Cubo base
	baseRenderItem = new RenderItem(CreateShape(PxBoxGeometry(3.0f, 0.5, 3)), &base, { 0.0, 0.0, 0.0, 1.0 });

	// Cubo tapa
	tapaRenderItem = new RenderItem(CreateShape(PxBoxGeometry(5.0f, 1, 5)), &tapa, { 0.0, 0.0, 0.0, 1.0 });

	// Cubo amarillo
	aireRenderItem = new RenderItem(CreateShape(PxBoxGeometry(5.0f, 5, 5)), &aire, { 1.0f, 1.0f, 0.22f, 1.0 });


	// Pelota de playa
	pelota = new Particle({ 0,30,0 }, { 0,0,0 }, { 0,0,0 }, 0.98, { 1.0f, 0.5f, 0.22f, 1.0f }, 5, 1);

	// Fuerza viento
	windForceGeneratorPelota = new WindForceGenerator({ 0.0f, 30.0f, 0.0f }, 0.99, 0, { 5.0f, 30.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, 30);
	pelota->addForceGenerator(windForceGeneratorPelota);
	pelota->addForceGenerator(gravityForceGenerator); // gravedad
	particulas.push_back(pelota);

	exp2_text = "Experimento 2: Columna de aire.";

}

void Game::experimentoFlotacionInit()
{
	// Vectores con posicion
	Vector3D baseVector = Vector3D(0, 60, 0);
	Vector3D tapaVector = Vector3D(0, 14, 0);

	// Transform
	base = PxTransform(baseVector.x(), baseVector.y(), baseVector.z());
	tapa = PxTransform(tapaVector.x(), tapaVector.y(), tapaVector.z());

	// Cubo base
	baseRenderItem = new RenderItem(CreateShape(PxBoxGeometry(5.0f, 1, 5)), &base, { 0.0, 0.0, 0.0, 1.0 });

	// Cubo tapa
	tapaRenderItem = new RenderItem(CreateShape(PxBoxGeometry(5.0f, 1, 5)), &tapa, { 0.0, 0.0, 0.0, 1.0 });


	// FLOTACION
	float y = 25.0;
	Particle* p4 = new Particle({ 0,y,0 }, { 0,0,0 }, { 0,0,0 }, 0.98, { 0.87, 0.3, 0.22, 1.0 }, 3, 30, CUBO);

	// fuerzas
	bouyancyForceGenerator = new BouyancyForceGenerator(y, 1000);
	p4->addForceGenerator(bouyancyForceGenerator);
	p4->addForceGenerator(gravityForceGenerator);

	particulas.push_back(p4);


	// Sistema de particulas burbujas
	ParticleSystem* pSystem = new ParticleSystem({ 0.0,24.0,0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.7, { 0.7, 0.7, 1.0, 0.0 }, 4, 0.2, 3, 1, SNOW);
	particleSystems.push_back(pSystem);

	// Force Generator de viento
	windForceGenerator = new WindForceGenerator({ 0.0f, 20.0f, 0.0f }, 0.99);
	pSystem->addForceGenerator(windForceGenerator);

	exp1_text = "Experimento 1: Barco a flote.";

}

void Game::experimentoBolasLokasInit()
{
	// Creo base
	baseEstatica = gPhysics->createRigidStatic(PxTransform({ 0,20,0 }));
	baseEstaticaShape = CreateShape(PxBoxGeometry(30, 5, 30));
	baseEstatica->attachShape(*baseEstaticaShape);
	gScene->addActor(*baseEstatica);
	baseEstaticaRenderItem = new RenderItem(baseEstaticaShape, baseEstatica, { 0,1,0.3,1 });

	RigidSolidSystem* sistmaSolidoRigidos = new RigidSolidSystem(gPhysics, gScene, { 25,25,0 }, { 0,0,0 }, 0.3, { 0,0,10 }, { 0,0,4 }, 0.5);
	RigidSolidSystem* sistmaSolidoRigidos2 = new RigidSolidSystem(gPhysics, gScene, { -25,25,0 }, { 0,0,0 }, 0.3, { 0,0,10 }, { 0,0,4 }, 0.5);
	RigidSolidSystem* sistmaSolidoRigidos3 = new RigidSolidSystem(gPhysics, gScene, { 0,25,25 }, { 0,0,0 }, 0.3, { 10,0,0 }, { 4,0,0 }, 0.5);
	RigidSolidSystem* sistmaSolidoRigidos4 = new RigidSolidSystem(gPhysics, gScene, { 0,25,-25 }, { 0,0,0 }, 0.3, { 10,0,0 }, { 4,0,0 }, 0.5);

	// Fuerza 1
	windForceGeneratorBolas1 = new WindForceGenerator({ -1000.0f, 0.0f, 0.0f }, 0.99, 0, { 24.5f, 18.5f, -15.5f }, { 28.5f, 27.0f, 16.5f });
	sistmaSolidoRigidos->addForceGenerator(windForceGeneratorBolas1);
	sistmaSolidoRigidos2->addForceGenerator(windForceGeneratorBolas1);
	sistmaSolidoRigidos3->addForceGenerator(windForceGeneratorBolas1);
	sistmaSolidoRigidos4->addForceGenerator(windForceGeneratorBolas1);

	// Fuerza 2
	windForceGeneratorBolas2 = new WindForceGenerator({ 1000.0f, 0.0f, 0.0f }, 0.99, 0, { -28.5f, 18.5f, -15.5f }, { -24.5f, 27.0f, 16.5f });
	sistmaSolidoRigidos->addForceGenerator(windForceGeneratorBolas2);
	sistmaSolidoRigidos2->addForceGenerator(windForceGeneratorBolas2);
	sistmaSolidoRigidos3->addForceGenerator(windForceGeneratorBolas2);
	sistmaSolidoRigidos4->addForceGenerator(windForceGeneratorBolas2);

	// Fuerza 3
	windForceGeneratorBolas3 = new WindForceGenerator({ 0.0f, 0.0f, -1000.0f }, 0.99, 0, { -16.f, 18.5f, 24.5f }, { 16.5f, 27.0f, 28.5f });
	sistmaSolidoRigidos->addForceGenerator(windForceGeneratorBolas3);
	sistmaSolidoRigidos2->addForceGenerator(windForceGeneratorBolas3);
	sistmaSolidoRigidos3->addForceGenerator(windForceGeneratorBolas3);
	sistmaSolidoRigidos4->addForceGenerator(windForceGeneratorBolas3);

	// Fuerza 4
	windForceGeneratorBolas4 = new WindForceGenerator({ 0.0f, 0.0f, 1000.0f }, 0.99, 0, { -16.f, 18.5f, -28.5f }, { 16.5f, 27.0f, -24.5f });
	sistmaSolidoRigidos->addForceGenerator(windForceGeneratorBolas4);
	sistmaSolidoRigidos2->addForceGenerator(windForceGeneratorBolas4);
	sistmaSolidoRigidos3->addForceGenerator(windForceGeneratorBolas4);
	sistmaSolidoRigidos4->addForceGenerator(windForceGeneratorBolas4);

	// Anado a vector de sistemas de solidos
	rigidSolidSystems.push_back(sistmaSolidoRigidos);
	rigidSolidSystems.push_back(sistmaSolidoRigidos2);
	rigidSolidSystems.push_back(sistmaSolidoRigidos3);
	rigidSolidSystems.push_back(sistmaSolidoRigidos4);

	exp4_text = "Experimento 4: Bolas lokas.";
}

void Game::experimentoExplosionMotorInit()
{
	// Creo figura de particulas
	// Anado generador de fuerzas de explosion
	blastForceGenerator = new BlastForceGenerator(50000, 60, { 0.0f,30.0f,0.0f }, 15);

	this->createCircleOfParticles({ 0.0f,30.0f,0.0f });

	// MUELLE DOS PARTICULAS MOVIENDOSE (Simulan valvulas)
	Particle* p1 = new Particle({ -30.0, 55.0,0.0 }, { 0.0, 0.0,0.0 }, { 0.0, 0.0,0.0 }, 0.85, { 0.0, 0.0,0.0, 1.0 }, 3, 2);
	Particle* p2 = new Particle({ -10.0, 45.0,0.0 }, { 0.0, 0.0,0.0 }, { 0.0, 0.0,0.0 }, 0.85, { 1.0, 1.0,1.0, 1.0 }, 3, 2);

	Particle* p4 = new Particle({ 30.0, 55.0,0.0 }, { 0.0, 0.0,0.0 }, { 0.0, 0.0,0.0 }, 0.85, { 0.0, 0.0,0.0, 1.0 }, 3, 2);
	Particle* p5 = new Particle({ 10.0, 44.0,0.0 }, { 0.0, 0.0,0.0 }, { 0.0, 0.0,0.0 }, 0.85, { 1.0, 1.0,1.0, 1.0 }, 3, 2);

	// Creo generadores de fuerza
	springForceGenerator1 = new SpringForceGenerator(1, 10, p2);
	springForceGenerator2 = new SpringForceGenerator(1, 10, p1);

	springForceGenerator3 = new SpringForceGenerator(1, 10, p5);
	springForceGenerator4 = new SpringForceGenerator(1, 10, p4);

	// Añado generadores de fuerza a las particulas
	p1->addForceGenerator(springForceGenerator1);
	p2->addForceGenerator(springForceGenerator2);

	p4->addForceGenerator(springForceGenerator3);
	p5->addForceGenerator(springForceGenerator4);

	// Añado particulas a la lista de particulas
	particulas.push_back(p1);
	particulas.push_back(p2);

	particulas.push_back(p4);
	particulas.push_back(p5);


	// MUELLE CON ANCLAJE (Simula piston)
	Particle* p3 = new Particle({ 0.0, 20.0,0.0 }, { 0.0, 0.0,0.0 }, { 0.0, 0.0,0.0 }, 0.85, { 0.0, 0.0,0.0, 1.0 }, 3, 1);
	anchoredSpringForceGenerator = new AnchoredSpringForceGenerator(1, 10, { 0.0,0.0, 0.0 });
	p3->addForceGenerator(anchoredSpringForceGenerator);
	p3->addForceGenerator(blastForceGenerator);
	particulas.push_back(p3);

	exp6_text = "Experimento 6: Cilindro motor.";
}

void Game::experimentoQuimicaInit()
{
	// DECORACION
	// Vectores con posicion
	Vector3D pataSize = { 2,10,2 };
	Vector3D boteSize = { 3.0f, 5, 3 };
	Vector3D boteVector = Vector3D(-37.5, 25, 27.5);
	Vector3D boteVector2 = Vector3D(20, 25, -37.5);
	Vector3D boteVector3 = Vector3D(-37.5, 25, -37.5);
	Vector3D mesaVector = Vector3D(-10, 20, -40);
	Vector3D pata1Vector = Vector3D(-27.5, 10, 32.5);
	Vector3D pata2Vector = Vector3D(-52.5, 10, 32.5);
	Vector3D pata3Vector = Vector3D(-52.5, 10, -52.5);
	Vector3D pata4Vector = Vector3D(-27.5, 10, -27.5);
	Vector3D pata5Vector = Vector3D(32.5, 10, -27.5);
	Vector3D pata6Vector = Vector3D(32.5, 10, -52.5);

	// Transform
	bote = PxTransform(boteVector.x(), boteVector.y(), boteVector.z());
	bote2 = PxTransform(boteVector2.x(), boteVector2.y(), boteVector2.z());
	bote3 = PxTransform(boteVector3.x(), boteVector3.y(), boteVector3.z());
	mesa = PxTransform(mesaVector.x(), mesaVector.y(), mesaVector.z());
	mesa2 = PxTransform(mesaVector.z(), mesaVector.y(), mesaVector.x());
	pata1 = PxTransform(pata1Vector.x(), pata1Vector.y(), pata1Vector.z());
	pata2 = PxTransform(pata2Vector.x(), pata2Vector.y(), pata2Vector.z());
	pata3 = PxTransform(pata3Vector.x(), pata3Vector.y(), pata3Vector.z());
	pata4 = PxTransform(pata4Vector.x(), pata4Vector.y(), pata4Vector.z());
	pata5 = PxTransform(pata5Vector.x(), pata5Vector.y(), pata5Vector.z());
	pata6 = PxTransform(pata6Vector.x(), pata6Vector.y(), pata6Vector.z());

	// Render Items
	boteRenderItem = new RenderItem(CreateShape(PxBoxGeometry(boteSize.x(), boteSize.y(), boteSize.z())), &bote, { 0.0, 0.0, 0.0, 1.0 });
	boteRenderItem2 = new RenderItem(CreateShape(PxBoxGeometry(boteSize.x(), boteSize.y(), boteSize.z())), &bote2, { 0.0, 0.0, 0.0, 1.0 });
	boteRenderItem3 = new RenderItem(CreateShape(PxBoxGeometry(boteSize.x(), boteSize.y(), boteSize.z())), &bote3, { 0.0, 0.0, 0.0, 1.0 });
	mesaRenderItem = new RenderItem(CreateShape(PxBoxGeometry(45.0f, 2, 15)), &mesa, { 0.423, 0.231, 0.165, 1.0 });
	mesaRenderItem2 = new RenderItem(CreateShape(PxBoxGeometry(15.0f, 2, 45)), &mesa2, { 0.423, 0.231, 0.165, 1.0 });
	pataRenderItem1 = new RenderItem(CreateShape(PxBoxGeometry(pataSize.x(), pataSize.y(), pataSize.z())), &pata1, { 0.423, 0.231, 0.165, 1.0 });
	pataRenderItem2 = new RenderItem(CreateShape(PxBoxGeometry(pataSize.x(), pataSize.y(), pataSize.z())), &pata2, { 0.423, 0.231, 0.165, 1.0 });
	pataRenderItem3 = new RenderItem(CreateShape(PxBoxGeometry(pataSize.x(), pataSize.y(), pataSize.z())), &pata3, { 0.423, 0.231, 0.165, 1.0 });
	pataRenderItem4 = new RenderItem(CreateShape(PxBoxGeometry(pataSize.x(), pataSize.y(), pataSize.z())), &pata4, { 0.423, 0.231, 0.165, 1.0 });
	pataRenderItem5 = new RenderItem(CreateShape(PxBoxGeometry(pataSize.x(), pataSize.y(), pataSize.z())), &pata5, { 0.423, 0.231, 0.165, 1.0 });
	pataRenderItem6 = new RenderItem(CreateShape(PxBoxGeometry(pataSize.x(), pataSize.y(), pataSize.z())), &pata6, { 0.423, 0.231, 0.165, 1.0 });

	// SISTEMAS DE PARTICULAS
	// ESPUMA
	ParticleSystem* pSystem = new ParticleSystem({ -37.5, 30,27.5 }, { 10.0, 10.0, 0.0 }, { 0.0, 10.0, 0.0 }, 4, { 1.0, 1.0, 1.0, 1.0 }, 9.0, 0.05, 5, 15, FOAM);
	particleSystems.push_back(pSystem);
	pSystem->addForceGenerator(gravityForceGenerator);

	// LANZADOR DE BOLAS
	ParticleSystem* pSystem2 = new ParticleSystem({ 20, 30, -37.5 }, { 0.0, 8.0, 0.0 }, { 0.0, 5.0, 0.0 }, 2, { 0.0, 1.0, 0.56, 1.0 }, 7.0, 0.5, 0, 10, BALLS);
	particleSystems.push_back(pSystem2);
	pSystem2->addForceGenerator(gravityForceGenerator);

	// BURBUJAS
	ParticleSystem* pSystem3 = new ParticleSystem({ -37.5,25.5,-37.5 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.7, { 1, 0.7, 1.0, 0.0 }, 8, 0.2, 1, 1, SNOW);
	particleSystems.push_back(pSystem3);
	windForceGenerator = new WindForceGenerator({ 0.0f, 10.0f, 0.0f }, 0.99);
	pSystem3->addForceGenerator(windForceGenerator);

	exp3_text = "Experimento 3: Quimica.";
}

void Game::experimentoGrifoInfinitoInit()
{
	// LLUVIA
	ParticleSystem* pSystem = new ParticleSystem({ 0.0,80.0,0.0 }, { 0.0, -10.0, 0.0 }, { 0.0, 20.0, 0.0 }, 0.25, { 0.0, 0.56, 0.9, 0.3 }, 9.0, 0.025, 60, 5, RAIN);
	particleSystems.push_back(pSystem);
	pSystem->addForceGenerator(gravityForceGenerator3);

	// Chorro de agua
	RigidSolidSystem* rSsys = new RigidSolidSystem(gPhysics, gScene, { -20,50,-20 }, { 0,-1,0 }, 0.75, { 0,0,0 }, { 0,0,0 }, 2, CUADRADO, { 1,1,1,1 });
	rigidSolidSystems.push_back(rSsys);

	// Cubo de agua
	baseEstatica = gPhysics->createRigidStatic(PxTransform({ -20,0,-20 }));
	baseEstaticaShape = CreateShape(PxBoxGeometry(10, 5, 10));
	baseEstatica->attachShape(*baseEstaticaShape);
	gScene->addActor(*baseEstatica);
	baseEstaticaRenderItem = new RenderItem(baseEstaticaShape, baseEstatica, { 0.5,0.5,1,1 });

	// Grifo
	Vector3D boteSize = { 2.5f, 1, 2.5 };
	Vector3D boteSize2 = { 3, 2, 6 };
	Vector3D boteSize3 = { 4, 4, 4 };
	Vector3D boteVector = Vector3D(-20.0, 52, -20);
	Vector3D boteVector2 = Vector3D(-20.0, 55, -22);
	Vector3D boteVector3 = Vector3D(-20.0, 57, -28);

	// Transform
	bote = PxTransform(boteVector.x(), boteVector.y(), boteVector.z());
	bote2 = PxTransform(boteVector2.x(), boteVector2.y(), boteVector2.z());
	bote3 = PxTransform(boteVector3.x(), boteVector3.y(), boteVector3.z());

	// Render Items
	boteRenderItem = new RenderItem(CreateShape(PxBoxGeometry(boteSize.x(), boteSize.y(), boteSize.z())), &bote, { 0.3, 0.3, 0.3, 1.0 });
	boteRenderItem2 = new RenderItem(CreateShape(PxBoxGeometry(boteSize2.x(), boteSize2.y(), boteSize2.z())), &bote2, { 0.5, 0.5, 0.5, 1.0 });
	boteRenderItem3 = new RenderItem(CreateShape(PxBoxGeometry(boteSize3.x(), boteSize3.y(), boteSize3.z())), &bote3, { 0.3, 0.3, 0.3, 1.0 });

	exp5_text = "Experimento 5: Grifo infinito.";

}

void Game::proyectilBala()
{
	// Bala
	Camera* cam = GetCamera();
	Vector3 cPos = cam->getEye();
	Vector3 cDir = cam->getDir();

	float speed = 50.0f;
	Projectile* p = new Projectile(cPos, cDir * speed, Vector3(0.0, -4.5, 0.0), 0.98, { 0.0, 1.0, 1.0, 1.0 }, 3, 1.0f);
	p->addForceGenerator(gravityForceGenerator);
	projectiles.push_back(p);
}

void Game::proyectilNieve()
{
	// Nieve
	Camera* cam = GetCamera();
	Vector3 cPos = cam->getEye();
	Vector3 cDir = cam->getDir();

	float speed = 7.0f;
	Projectile* p = new Projectile(cPos, cDir * speed, Vector3(0.0, -1.5, 0.0), 0.98, { 1.0, 1.0, 1.0, 1.0 }, 3, 1.0f);
	p->addForceGenerator(gravityForceGenerator);
	projectiles.push_back(p);
}

void Game::proyectilBola()
{
	// Bola
	Camera* cam = GetCamera();
	Vector3 cPos = cam->getEye();
	Vector3 cDir = cam->getDir();

	float speed = 25.0f;
	Projectile* p = new Projectile(cPos, cDir * speed, Vector3(0.0, -4.5, 0.0), 0.98, { 1.0, 1.0, 0.0, 1.0 }, 2, 1.0f);
	p->addForceGenerator(gravityForceGenerator);
	projectiles.push_back(p);
}
