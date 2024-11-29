#include "BlastForceGenerator.h"

BlastForceGenerator::BlastForceGenerator(float intensity, float blastRadio, Vector3 center, float r, bool visible) :ForceGenerator(center, { 0.0f,0.0f ,0.0f }, r, visible)
{
	centerPos = center;
	k = intensity;
	this->radio = blastRadio;
	time = 0;
	active = false;
}

void BlastForceGenerator::update(Particle* p, double t)
{
	if (active) {
		Vector3 particlePos = p->getPos();
		if (usingBounding && boundingShape->isInside(particlePos) || !usingBounding) {

			time += t;
			Vector3 particlePos = p->getPos();
			Vector3 distV = particlePos - centerPos;
			float dist = distV.magnitude();
			if (dist <= radio) {
				force = k * distV / pow(dist, 2) * exp(-time / timeConst);
				p->addForce(force);
			}

		}
	}

	//time += t;
	//float distDiff = (particlePos - centerPos).magnitude();
	//const float timeConst = 20.0f;
	//if (time < 4 * timeConst) {
	//	r = velocity.magnitude() * time;

	//	force = (k / (distDiff * distDiff)) * (particlePos - centerPos) * exp(-time / timeConst);
	//	if (distDiff < r) p->addForce(force);
	//}
}

void BlastForceGenerator::activateBlast()
{
	active = true;
}