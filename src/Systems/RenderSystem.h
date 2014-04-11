#ifndef RenderSystem_h__
#define RenderSystem_h__

#include <unordered_map>

#include "System.h"
#include "Systems/TransformSystem.h"
#include "Model.h"
#include "Texture.h"
#include "Components/Model.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Components/Bounds.h"
#include "Components/Collision.h"
#include "Renderer.h"

namespace Systems
{

class  RenderSystem : public System
{
public:
	RenderSystem(World* world, std::shared_ptr<Renderer> renderer)
		: System(world), m_Renderer(renderer) { }

	void Initialize() override;

	std::unordered_map<std::string, std::shared_ptr<Model>> m_CachedModels;

	void OnComponentCreated(std::string type, std::	shared_ptr<Component> component) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;

private:
	std::shared_ptr<Renderer> m_Renderer;
	std::shared_ptr<Systems::TransformSystem> m_TransformSystem;
};


}




#endif //RenderSystem_h__