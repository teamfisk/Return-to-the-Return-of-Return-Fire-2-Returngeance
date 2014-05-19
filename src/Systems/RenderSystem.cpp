#include "PrecompiledHeader.h"
#include "RenderSystem.h"
#include "World.h"

void Systems::RenderSystem::RegisterResourceTypes(ResourceManager* rm)
{
	rm->RegisterType("Model", [rm](std::string resourceName) { return new Model(rm, *rm->Load<OBJ>("OBJ", resourceName)); });
	rm->RegisterType("OBJ", [](std::string resourceName) { return new OBJ(resourceName); });
	rm->RegisterType("Texture", [](std::string resourceName) { return new Texture(resourceName); });
}

void Systems::RenderSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register<Components::Camera>([]() { return new Components::Camera(); });
	cf->Register<Components::Model>([]() { return new Components::Model(); });
	cf->Register<Components::Sprite>([]() { return new Components::Sprite(); });
	cf->Register<Components::PointLight>([]() { return new Components::PointLight(); });
	cf->Register<Components::DirectionalLight>([]() { return new Components::DirectionalLight(); });
	cf->Register<Components::Viewport>([]() { return new Components::Viewport(); });
}

void Systems::RenderSystem::OnEntityCommit(EntityID entity)
{
	auto transform = m_World->GetComponent<Components::Transform>(entity);

	auto camera = m_World->GetComponent<Components::Camera>(entity);
	if (transform && camera)
	{
		m_Renderer->RegisterCamera(entity, camera->FOV, camera->NearClip, camera->FarClip);
		m_Renderer->UpdateCamera(entity, m_TransformSystem->AbsolutePosition(entity), m_TransformSystem->AbsoluteOrientation(entity), camera->FOV, camera->NearClip, camera->FarClip);
	}

	auto viewport = m_World->GetComponent<Components::Viewport>(entity);
	if (viewport)
	{
		m_Renderer->RegisterViewport(entity, viewport->Left, viewport->Top, viewport->Right, viewport->Bottom);
		if (viewport->Camera != 0)
		{
			m_Renderer->UpdateViewport(entity, viewport->Camera);
		}
	}
}

void Systems::RenderSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity);

	// Draw models
	auto modelComponent = m_World->GetComponent<Components::Model>(entity);
	if (transformComponent&& modelComponent)
	{
		auto model = m_World->GetResourceManager()->Load<Model>("Model", modelComponent->ModelFile);
		if (model)
		{
			/*glm::vec3 position = m_TransformSystem->AbsolutePosition(entity);
			glm::quat orientation = m_TransformSystem->AbsoluteOrientation(entity);
			glm::vec3 scale = m_TransformSystem->AbsoluteScale(entity);*/
			Components::Transform absoluteTransform = m_TransformSystem->AbsoluteTransform(entity);
			m_Renderer->AddModelToDraw(model, absoluteTransform.Position, absoluteTransform.Orientation, absoluteTransform.Scale, modelComponent->Visible, modelComponent->ShadowCaster);
		}
	}

	auto pointLightComponent = m_World->GetComponent<Components::PointLight>(entity);
	if (transformComponent && pointLightComponent)
	{
		glm::vec3 position = m_TransformSystem->AbsolutePosition(entity);
		m_Renderer->AddPointLightToDraw(
			position,
			pointLightComponent->Specular,
			pointLightComponent->Diffuse,
			pointLightComponent->specularExponent,
			pointLightComponent->ConstantAttenuation,
			pointLightComponent->LinearAttenuation,
			pointLightComponent->QuadraticAttenuation
			);
	}

	auto cameraComponent = m_World->GetComponent<Components::Camera>(entity);
	if (transformComponent && cameraComponent)
	{
		m_Renderer->UpdateCamera(entity
			, m_TransformSystem->AbsolutePosition(entity)
			, m_TransformSystem->AbsoluteOrientation(entity)
			, cameraComponent->FOV
			, cameraComponent->NearClip
			, cameraComponent->FarClip);
	}

	auto viewportComponent = m_World->GetComponent<Components::Viewport>(entity);
	if (viewportComponent)
	{
		if (viewportComponent->Camera != 0)
		{
			m_Renderer->UpdateViewport(entity, viewportComponent->Camera);
		}
	}

	auto spriteComponent = m_World->GetComponent<Components::Sprite>(entity);
	if (transformComponent && spriteComponent)
	{
		//TEMP
		Texture* texture = m_World->GetResourceManager()->Load<Texture>("Texture", spriteComponent->SpriteFile);
		//glBindTexture(GL_TEXTURE_2D, texture);
		auto transform = m_World->GetComponent<Components::Transform>(spriteComponent->Entity);
		glm::quat orientation2D = glm::angleAxis(glm::eulerAngles(transform->Orientation).z, glm::vec3(0, 0, -1));
		m_Renderer->AddTextureToDraw(texture, transform->Position, orientation2D, transform->Scale);
	}
}

void Systems::RenderSystem::Initialize()
{
	m_TransformSystem = m_World->GetSystem<Systems::TransformSystem>();

	m_Renderer->SetSphereModel(m_World->GetResourceManager()->Load<Model>("Model", "Models/Placeholders/PhysicsTest/Sphere.obj"));
}

