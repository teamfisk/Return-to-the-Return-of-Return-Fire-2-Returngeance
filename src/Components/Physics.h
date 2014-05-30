#ifndef Components_Physics_h__
#define Components_Physics_h__

#include "Component.h"

namespace Components
{

struct Physics : Component
{
	Physics()
		: Mass(1.f), Static(false), Phantom(false), CalculateCenterOfMass(true), CenterOfMass(glm::vec3(0)), InitialLinearVelocity(glm::vec3(0)), InitialAngularVelocity(glm::vec3(0)),
	LinearDamping(0.f), AngularDamping(0.05f), GravityFactor(1.f), Friction(0.5f), Restitution(0.4f), MaxLinearVelocity(200.f), MaxAngularVelocity(200.f), 
	CollisionLayer(0), CollisionSystemGroup(0), CollisionSubSystemId(0), CollisionSubSystemDontCollideWith(0), CollisionEvent(false){}

	float Mass;
	bool Static;
	bool Phantom;

	bool CalculateCenterOfMass;
	glm::vec3 CenterOfMass;
	glm::vec3 InitialLinearVelocity;
	glm::vec3 InitialAngularVelocity;
	float LinearDamping;
	float AngularDamping;
	float GravityFactor;
	float Friction;
	float Restitution;
	float MaxLinearVelocity;
	float MaxAngularVelocity;
	
	int CollisionLayer;
	int CollisionSystemGroup;
	int CollisionSubSystemId;
	int CollisionSubSystemDontCollideWith;
	
	bool CollisionEvent;

	virtual Physics* Clone() const override { return new Physics(*this); }
};

}

#endif // Components_Physics_h__
