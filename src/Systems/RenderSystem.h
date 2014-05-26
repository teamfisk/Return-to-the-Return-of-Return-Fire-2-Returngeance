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
#include "RenderQueue.h"

namespace Systems
{

class  RenderSystem : public System
{
public:
	RenderSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
		: System(world, eventBroker, resourceManager)
	{ }

	void RegisterComponents(ComponentFactory* cf) override;
	void RegisterResourceTypes(std::shared_ptr<::ResourceManager> rm) override;
	void Initialize() override;

	std::unordered_map<std::string, std::shared_ptr<Model>> m_CachedModels;

	void OnEntityCommit(EntityID entity) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;

private:
	std::shared_ptr<Systems::TransformSystem> m_TransformSystem;

	void EnqueueModel(Model* model, glm::mat4 modelMatrix);
	void EnqueueSprite(Texture* texture, glm::mat4 modelMatrix);

};


}




#endif //RenderSystem_h__