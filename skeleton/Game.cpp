#include "Game.h"
#include "Vector3D.h"

Game::Game(physx::PxPhysics* physics, physx::PxScene* scene)
{
	gPhysics = physics;
	gScene = scene;

	_state = START;
	_nextState = START;

	// Gravedad
	gravityForceGenerator = new GravityForceGenerator(10.0f);
	gravityForceGenerator2 = new GravityForceGenerator(10.0f);

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

		switch (_state)
		{
		case START: {
			std::cout << "Start State" << endl;
			deleteAll();
			deleteForces();
			// Gravedad
			if (gravityForceGenerator == nullptr)
				gravityForceGenerator = new GravityForceGenerator(10.0f);
			if (gravityForceGenerator2 == nullptr)
				gravityForceGenerator2 = new GravityForceGenerator(10.0f);

			startInit();
			break;
		}
		case E_FLOTACION: {
			std::cout << "Flotacion State" << endl;
			deleteAll();
			deleteSpecificForces();
			experimentoFlotacionInit();

			break;
		}
		case E_COLUMNA_AIRE: {
			std::cout << "Columna de aire State" << endl;
			deleteAll();
			deleteSpecificForces();
			experimentoColumnaAireInit();
			break;
		}
		case E_BOLAS_LOKAS: {
			std::cout << "Bolas lokas State" << endl;
			deleteAll();
			deleteSpecificForces();
			experimentoBolasLokasInit();
			break;
		}
		case E_EXPLOSION_MOTOR:
		{
			std::cout << "Motor State" << endl;
			deleteAll();
			deleteSpecificForces();
			experimentoExplosionMotorInit();
			break;
		}
		case E_QUIMICA: {
			std::cout << "Motor State" << endl;
			deleteAll();
			deleteSpecificForces();
			experimentoQuimicaInit();
			break;
		}
		case END: {

			std::cout << "End State" << endl;
			deleteAll();
			deleteForces();
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
	switch (_state)
	{
	case START:
	case E_FLOTACION:
	case E_COLUMNA_AIRE:
	case E_BOLAS_LOKAS:
	case E_QUIMICA:
	case END:
		if (!_pause)
			updateElements(t);

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
		// Explosion Rigid SOlid
		RigidSolidSystem* rsS = new RigidSolidSystem(gPhysics, gScene, { 0,50,0 }, { 0,0,0 }, 0.25, { 10,0,10 }, { 10,10,10 }, 2, BASIC, { 1,0,0,1 });
		rsS->addForceGenerator(blastForceGenerator);
		rigidSolidSystems.push_back(rsS);
		break;
	}
	case 'T': {

		// LLUVIA
		ParticleSystem* pSystem = new ParticleSystem({ 0.0,50.0,0.0 }, { 0.0, -10.0, 0.0 }, { 0.0, 20.0, 0.0 }, 0.25, { 0.0, 0.56, 0.9, 0.3 }, 9.0, 0.05, 20, 5, RAIN);
		particleSystems.push_back(pSystem);
		pSystem->addForceGenerator(gravityForceGenerator);


		break;
	}
	case 'E': {
		// Rigid solid cuadrado
		RigidSolid* rs = new RigidSolid(gPhysics, gScene, 15, { 1,1,-1 }, { 0.0,50.0,0.0 }, { 1.0, 1.0, 1.0, 1.0 }, 5, 2, BOX_RS);
		rs->addForceGenerator(gravityForceGenerator2);
		rigidSolids.push_back(rs);

		break;
	}
	case 'F':
	{
		// Invierte gravedad
		gravityForceGenerator2->changeGravity(-gravityForceGenerator2->getGravity());
		break;
	}

	case 'C':
	{
		// Creo figura de particulas
		// Añado generador de fuerzas de explosion
		this->createCircleOfParticles({ 0.0f,50.0f,0.0f });

		break;
	}

	case 'X': {
		Particle* p = new Particle({ 0.0f,50.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, 0.98, { 0.0,0.0,0.0,1.0 }, 0.2, 2);
		particulas.push_back(p);
		blastForceGenerator->activateBlast();

		break;
	}
	case 'Z': {
		// MUELLE DOS PARTICULAS MOVIENDOSE
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


		// MUELLE CON ANCLAJE
		Particle* p3 = new Particle({ -10.0, 50.0,0.0 }, { 0.0, 0.0,0.0 }, { 0.0, 0.0,0.0 }, 0.85, { 0.0, 0.0,0.0, 1.0 }, 2, 1);
		anchoredSpringForceGenerator = new AnchoredSpringForceGenerator(1, 10, { 10.0,60.0, 0.0 });
		p3->addForceGenerator(anchoredSpringForceGenerator);
		p3->addForceGenerator(gravityForceGenerator2);
		particulas.push_back(p3);

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
	// LANZADOR DE BOLAS
	ParticleSystem* pSystem = new ParticleSystem({ 0.0,10.0,0.0 }, { 0.0, 8.0, 0.0 }, { 0.0, 5.0, 0.0 }, 2, { 0.0, 1.0, 0.56, 1.0 }, 7.0, 0.5, 5, 15, BALLS);
	particleSystems.push_back(pSystem);
	pSystem->addForceGenerator(gravityForceGenerator);

	// BOLAS COLORIDAS
	ParticleSystem* pSystem2 = new ParticleSystem({ 0.0,50.0,0.0 }, { 10.0, 10.0, 0.0 }, { 0.0, 10.0, 0.0 }, 3, { 1.0, 1.0, 1.0, 1.0 }, 9.0, 0.05, 5, 15, COLOURFULL);
	particleSystems.push_back(pSystem2);
	pSystem2->addForceGenerator(gravityForceGenerator2);
}

void Game::endInit()
{
	// Vientoooo
	ParticleSystem* pSystem = new ParticleSystem({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 3, { 0,0,0, 0 }, 10, 0.5, 10, 0, COLOURFULL);
	particleSystems.push_back(pSystem);
	windForceGenerator = new WindForceGenerator({ 0.0f, 20.0f, 0.0f }, 0.99);
	pSystem->addForceGenerator(windForceGenerator);


	// Tornadoo
	ParticleSystem* pSystem2 = new ParticleSystem({ 30.0f, 30.0f, 9.0f }, { 20.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 7, { 0.0f, 0.0f, 0.0f,  0.0f }, 10, 0.05, 10, 10, COLOURFULL);
	particleSystems.push_back(pSystem2);
	pSystem2->addForceGenerator(tornadoForceGenerator);
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
}

void Game::experimentoBolasLokasInit()
{
	// Creo suelo
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
}

void Game::experimentoExplosionMotorInit()
{
	blastForceGenerator = new BlastForceGenerator(50000, 60, { 0.0f,50.0f,0.0f });
}

void Game::experimentoQuimicaInit()
{
	// ESPUMA
	ParticleSystem* pSystem = new ParticleSystem({ 0.0,50.0,0.0 }, { 10.0, 10.0, 0.0 }, { 0.0, 10.0, 0.0 }, 4, { 1.0, 1.0, 1.0, 1.0 }, 9.0, 0.05, 5, 15, FOAM);
	particleSystems.push_back(pSystem);
	pSystem->addForceGenerator(gravityForceGenerator);
}

void Game::lanzaProyectiles()
{
	// if 1 lanza proyectil 1
	proyectilBala();

	// if 2 lanza proyectil 2
	proyectilBola();

	// if 3 lanza proyectil 3
	proyectilNieve();

	// Tengo q mirar como funciona el click del raton, sino lo hago con estados


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
