/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "fieldkit/physics/Physics.h"

using namespace fieldkit::physics;

Physics::Physics(SpacePtr space) 
{
	this->space = space;
	doUpdateNeighbours = true;
	emptySpaceOnUpdate = true;
	numParticles = 0;
}

Physics::~Physics() 
{
	particles.clear();
	springs.clear();
}

void Physics::update(float dt)
{
	if(emitter)
		emitter->update(dt);
	
	updateParticles(dt);
	updateSprings();
	updateNeighbours();
}

// -- Particles ------------------------------------------------------------
// check if we still have a dead particle that we can reuse, otherwise create a new one
ParticlePtr Physics::createParticle() 
{
	numParticles++;
	//for(ParticlePtr pIt = particles.begin(); pIt != particles.end();) {
	BOOST_FOREACH(ParticlePtr p, particles) {
		if(!p->isAlive) return p;
	}
	
	logger() << "WARNING: running out of particles" << std::endl;
	// FIXME
	return particles[0];
}

// allocates a bunch of new particles
void Physics::allocParticles(int count) 
{
	particles.reserve(count);
	for(int i=0; i<count; i++){
		particles.push_back( allocParticle() );
	}
	
	space->reserve(count);
}

// allocates a single particle, override this method to create custom Particle types
ParticlePtr Physics::allocParticle()
{
	return new Particle();
}

// updates all particles by applying all behaviours and constraints
void Physics::updateParticles(float dt) 
{
	// prepare behaviours & constraints
	BOOST_FOREACH(BehaviourPtr b, behaviours) {
		b->prepare(dt);
	}
	
	BOOST_FOREACH(ConstraintPtr c, constraints) {
		c->prepare(dt);
	}
	
	// update all particles
	//for(ParticlePtr p = particles.begin(); p != particles.end(); p++) {
	BOOST_FOREACH(ParticlePtr p, particles) {
		if(!p->isAlive) continue;
		
		// apply behaviours
		BOOST_FOREACH(BehaviourPtr b, behaviours) {
			b->apply(p);
		}
		
		// apply constraints
		BOOST_FOREACH(ConstraintPtr c, constraints) {
			c->apply(p);
		}
		
		// update particle
		p->update(dt);
		
		// particle just has died
		if(!p->isAlive) {
			numParticles--;
		}
	}
}


// -- Springs --------------------------------------------------------------
void Physics::addSpring(SpringPtr spring) 
{
	springs.push_back(spring);
}

void Physics::removeSpring(SpringPtr spring)
{
	// TODO
//	springs.erase(spring);
}

// updates all spring connections based on new particle positions
void Physics::updateSprings() 
{
	// update all springs
	BOOST_FOREACH(SpringPtr s, springs) {
		s->update();
		
		// apply constraints after spring update
		BOOST_FOREACH(ConstraintPtr c, constraints) {
			c->apply(s->a);
			c->apply(s->b);
		}			
	}		
}

// -- Neighbours -----------------------------------------------------------
void Physics::updateNeighbours()
{
	if(!doUpdateNeighbours) return;

	if(emptySpaceOnUpdate) 
		space->clear();

	BOOST_FOREACH(ParticlePtr p, particles) {
		if(p->isAlive)
			space->insert(p);
	}

	BOOST_FOREACH(ParticlePtr p, particles) {
		if(p->isAlive)
			space->select(p->getBounds(), p->getNeighbours());
	}
}
