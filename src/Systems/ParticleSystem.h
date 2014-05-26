#ifndef ParticleSystem_h__
#define ParticleSystem_h__

#include "System.h"
#include "Systems/TransformSystem.h"
#include "Components/Transform.h"
#include "Components/ParticleEmitter.h"
#include "Components/Particle.h"
#include "Components/Model.h"
#include "Components/PointLight.h"
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
	std::shared_ptr<Systems::TransformSystem> m_TransformSystem;

	
};

}


#endif // !ParticleSystem_h__