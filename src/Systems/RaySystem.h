#ifndef DebugSystem_h__
#define DebugSystem_h__

#include "System.h"
#include "Events/CastRay.h"

namespace Systems
{

class RaySystem : public System
{
public:
	RaySystem(World* world, std::shared_ptr<::EventBroker> eventBroker)
		: System(world, eventBroker) { }

	void Initialize() override;

	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;

private:
	struct Ray
	{
		glm::vec3 Direction;
	};

	EventRelay<Events::CastRay> m_ECastRay;
	bool OnCastRay(const Events::CastRay &event);

	std::list<Ray> m_UnresolvedRays;
	//void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
};

}
#endif // DebugSystem_h__