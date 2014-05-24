#ifndef RenderSystem_h__
#define RenderSystem_h__

#include <unordered_map>

#include "System.h"
#include "Systems/TransformSystem.h"
#include "Model.h"
#include "Texture.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Components/Model.h"
#include "Components/Sprite.h"
#include "Components/PointLight.h"
#include "Components/DirectionalLight.h"
#include "Components/Viewport.h"

#include "Components/Template.h"
#include "Components/Transform.h"
#include "Renderer.h"

namespace Systems
{

class  RenderSystem : public System
{
public:
	RenderSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<Renderer> renderer)
		: System(world, eventBroker)
		, m_Renderer(renderer) { }

	void RegisterComponents(ComponentFactory* cf) override;
	void RegisterResourceTypes(ResourceManager* rm) override;
	void Initialize() override;

	std::unordered_map<std::string, std::shared_ptr<Model>> m_CachedModels;

	void OnEntityCommit(EntityID entity) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;




private:
	std::shared_ptr<Renderer> m_Renderer;
	std::shared_ptr<Systems::TransformSystem> m_TransformSystem;
};


}




#endif //RenderSystem_h__