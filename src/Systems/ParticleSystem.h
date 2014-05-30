#ifndef ParticleSystem_h__
#define ParticleSystem_h__

#include "System.h"
#include "Systems/TransformSystem.h"
#include "Components/Transform.h"
#include "Components/ParticleEmitter.h"
#include "Components/Particle.h"
#include "Components/Sprite.h"
#include "EventBroker.h"
#include "Events/KeyUp.h"
#include "Events/CreateExplosion.h"
#include "Color.h"
#include <GLFW/glfw3.h>

namespace Systems
{

	struct ParticleData
	{
		EntityID ParticleID;
		double SpawnTime;
		float AngularVelocity;
		glm::vec3 Orientation;
		Color color;
	};

class ParticleSystem : public System
{
public:
	ParticleSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
		: System(world, eventBroker, resourceManager)
	{ }

	void RegisterComponents(ComponentFactory* cf) override;
	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
	void Initialize() override;

	//void CreateExplosion(glm::vec3 _pos, double _lifeTime, int _particlesToSpawn, std::string _spritePath, glm::quat _relativeUpOri, float _speed, float _spreadAngle, float _particleScale);

	virtual bool OnCommand(const Events::KeyUp &event) { return false; }

private:
	void SpawnParticles(EntityID emitterID);
	float RandomizeAngle(float spreadAngle);
	//void ScaleInterpolation(double timeProgress, std::vector<float> spectrum, glm::vec3 &scale);
	void VectorInterpolation(double timeProgress, std::vector<glm::vec3> spectrum, glm::vec3 &velocity);
	//void ColorInterpolation(double timeProgress, std::vector<Color> spectrum, Color &color);
	void ScalarInterpolation(double timeProgress, std::vector<float> spectrum, float &alpha);
	void Billboard();
	std::map<EntityID, std::list<ParticleData>> m_ParticleEmitter;
	std::map<EntityID, double> m_TimeSinceLastSpawn;
	std::map<EntityID, double> m_ExplosionEmitters;
	std::shared_ptr<Systems::TransformSystem> m_TransformSystem;

	bool tempSpawnedExplosions;
	
// 	EventRelay<ParticleSystem, Events::KeyUp> m_EKeyUp;
// 	bool OnKeyUp(const Events::KeyUp &e);
	EventRelay<ParticleSystem, Events::CreateExplosion> m_EExplosion;
	bool CreateExplosion(const Events::CreateExplosion &e);

};

}


#endif // !ParticleSystem_h__