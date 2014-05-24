#ifndef GUI_Viewport_h__
#define GUI_Viewport_h__

#include <memory>

#include "GUI/Frame.h"
#include "World.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "RenderQueue.h"
#include "Camera.h"

namespace GUI
{

class Viewport : public Frame
{
public:
	Viewport(std::shared_ptr<Frame> parent, std::string name, std::shared_ptr<World> world)
		: Frame(parent, name)
		, m_World(world)
	{ }

	EntityID CameraEntity() const { return m_CameraEntity; }
	void SetCameraEntity(EntityID cameraEntity) 
	{ 
		m_CameraEntity = cameraEntity;
		auto transformComponent = m_World->GetComponent<Components::Transform>(cameraEntity);
		if (!transformComponent)
			return;
		auto cameraComponent = m_World->GetComponent<Components::Camera>(cameraEntity);
		if (!cameraComponent)
			return;

		m_Camera = std::make_shared<Camera>(cameraComponent->FOV, cameraComponent->NearClip, cameraComponent->FarClip);
	}

	void Initialize() override
	{

	}

	void Update(double dt) override
	{
		auto transformComponent = m_World->GetComponent<Components::Transform>(m_CameraEntity);
		if (!transformComponent)
			return;
		auto cameraComponent = m_World->GetComponent<Components::Camera>(m_CameraEntity);
		if (!cameraComponent)
			return;

		m_Camera->SetFOV(cameraComponent->FOV);
		m_Camera->SetNearClip(cameraComponent->NearClip);
		m_Camera->SetFarClip(cameraComponent->FarClip);
	}

	void Draw(std::shared_ptr<Renderer> renderer) override
	{
		renderer->SetCamera(m_Camera);

		Frame::Draw(renderer);
	}

private:
	std::shared_ptr<World> m_World;
	std::shared_ptr<Camera> m_Camera;
	EntityID m_CameraEntity;
	
};

}

#endif // GUI_Viewport_h__
