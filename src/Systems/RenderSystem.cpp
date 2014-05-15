#include "PrecompiledHeader.h"
#include "RenderSystem.h"
#include "World.h"

void Systems::RenderSystem::OnComponentCreated(std::string type, std::shared_ptr<Component> component)
{
	if(type == "Model")
	{
		auto modelComponent = std::static_pointer_cast<Components::Model>(component);
	}
}

void Systems::RenderSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
	if (transformComponent == nullptr)
		return;

	// Draw models
	auto modelComponent = m_World->GetComponent<Components::Model>(entity, "Model");
	if (modelComponent != nullptr)
	{
		auto model = m_World->GetResourceManager()->Load<Model>("Model", modelComponent->ModelFile);
		if (model != nullptr)
		{
			/*glm::vec3 position = m_TransformSystem->AbsolutePosition(entity);
			glm::quat orientation = m_TransformSystem->AbsoluteOrientation(entity);
			glm::vec3 scale = m_TransformSystem->AbsoluteScale(entity);*/
			Components::Transform absoluteTransform = m_TransformSystem->AbsoluteTransform(entity);
			m_Renderer->AddModelToDraw(model, absoluteTransform.Position, absoluteTransform.Orientation, absoluteTransform.Scale, modelComponent->Visible, modelComponent->ShadowCaster);
		}
	}

	auto pointLightComponent = m_World->GetComponent<Components::PointLight>(entity, "PointLight");
	if (pointLightComponent != nullptr)
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

	auto cameraComponent = m_World->GetComponent<Components::Camera>(entity, "Camera");
	if (cameraComponent != nullptr)
	{
		m_Renderer->GetCamera()->Position(m_TransformSystem->AbsolutePosition(entity));
		m_Renderer->GetCamera()->Orientation(m_TransformSystem->AbsoluteOrientation(entity));

		m_Renderer->GetCamera()->FOV(cameraComponent->FOV);
		m_Renderer->GetCamera()->NearClip(cameraComponent->NearClip);
		m_Renderer->GetCamera()->FarClip(cameraComponent->FarClip);
	}

	auto spriteComponent = m_World->GetComponent<Components::Sprite>(entity, "Sprite");
	if(spriteComponent != nullptr)
	{
		//TEMP
		Texture* texture = m_World->GetResourceManager()->Load<Texture>("Texture", spriteComponent->SpriteFile);
		//glBindTexture(GL_TEXTURE_2D, texture);
		auto transform = m_World->GetComponent<Components::Transform>(spriteComponent->Entity, "Transform");
		glm::quat orientation2D = glm::angleAxis(glm::eulerAngles(transform->Orientation).z, glm::vec3(0, 0, -1));
		m_Renderer->AddTextureToDraw(texture, transform->Position, orientation2D, transform->Scale);
	}
}

void Systems::RenderSystem::Initialize()
{
	m_TransformSystem = m_World->GetSystem<Systems::TransformSystem>("TransformSystem");

	m_Renderer->SetSphereModel(m_World->GetResourceManager()->Load<Model>("Model", "Models/Placeholders/PhysicsTest/Sphere.obj"));
}

void Systems::RenderSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register("Camera", []() { return new Components::Camera(); });
	cf->Register("Model", []() { return new Components::Model(); });
	cf->Register("Sprite", []() { return new Components::Sprite(); });
	cf->Register("PointLight", []() { return new Components::PointLight(); });
	cf->Register("DirectionalLight", []() { return new Components::DirectionalLight(); });
}

void Systems::RenderSystem::RegisterResourceTypes(ResourceManager* rm)
{
	rm->RegisterType("Model", [rm](std::string resourceName) { return new Model(rm, *rm->Load<OBJ>("OBJ", resourceName)); });
	rm->RegisterType("OBJ", [](std::string resourceName) { return new OBJ(resourceName); });
	rm->RegisterType("Texture", [](std::string resourceName) { return new Texture(resourceName); });
}





