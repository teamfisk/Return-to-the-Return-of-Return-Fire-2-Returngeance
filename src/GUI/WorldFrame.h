#ifndef GUI_WorldFrame_h__
#define GUI_WorldFrame_h__

#include "GUI/Frame.h"
#include "GUI/Viewport.h"
#include "RenderQueue.h"
#include "World.h"
#include "Systems/TransformSystem.h"
#include "Events/SetViewportCamera.h"
#include "Components/Transform.h"
#include "Components/Model.h"
#include "Components/Sprite.h"
#include "Components/PointLight.h"

namespace GUI
{

	class WorldFrame : public Frame
	{
	public:
		WorldFrame(std::shared_ptr<Frame> parent, std::string name, std::shared_ptr<World> world)
			: Frame(parent, name)
			, m_World(world)
		{ }

		void Initialize() override
		{
			EVENT_SUBSCRIBE_MEMBER(m_ESetViewportCamera, &WorldFrame::OnSetViewportCamera);

			m_World->Initialize();
		}

		void Update(double dt) override
		{
			m_World->Update(dt);
			m_TransformSystem = m_World->GetSystem<Systems::TransformSystem>();
		}

		void Draw(std::shared_ptr<Renderer> renderer) override
		{
			renderer->Flush();

			RenderQueue<ModelJob> modelQueue;
			RenderQueue<SpriteJob> spriteQueue;
			for (auto &pair : *m_World->GetEntities())
			{
				EntityID entity = pair.first;

				auto transform = m_World->GetComponent<Components::Transform>(entity);
				if (!transform)
					continue;

				auto modelComponent = m_World->GetComponent<Components::Model>(entity);
				if (modelComponent)
				{
					auto modelAsset = ResourceManager->Load<Model>("Model", modelComponent->ModelFile);
					if (modelAsset)
					{
						Components::Transform absoluteTransform = m_TransformSystem->AbsoluteTransform(entity);
						glm::mat4 modelMatrix = glm::translate(glm::mat4(), absoluteTransform.Position)
										      * glm::toMat4(absoluteTransform.Orientation)
											  * glm::scale(absoluteTransform.Scale);
						EnqueueModel(modelQueue, modelAsset, modelMatrix);
					}
				}

				auto spriteComponent = m_World->GetComponent<Components::Sprite>(entity);
				if (spriteComponent)
				{
					auto textureAsset = ResourceManager->Load<Texture>("Texture", spriteComponent->SpriteFile);
					if (textureAsset)
					{
						Components::Transform absoluteTransform = m_TransformSystem->AbsoluteTransform(entity);
						glm::quat orientation2D = glm::angleAxis(glm::eulerAngles(absoluteTransform->Orientation).z, glm::vec3(0, 0, -1));
						glm::mat4 modelMatrix = glm::translate(absoluteTransform.Position);
											  * glm::toMat4(orientation2D)
											  * glm::scale(absoluteTransform.Scale);
						EnqueueSprite(spriteQueue, textureAsset, modelMatrix);
					}
				}

				auto pointLightComponent = m_World->GetComponent<Components::PointLight>(entity);
				if (pointLightComponent)
				{
					// TODO: Push lights to renderer
					// renderer->PushLight(...);
				}
			}

			renderer->PushQueue(rq);

			Frame::Draw(renderer);
		}

	protected:
		std::shared_ptr<World> m_World;

	private:
		EventRelay<Frame, Events::SetViewportCamera> m_ESetViewportCamera;
		bool OnSetViewportCamera(const Events::SetViewportCamera &event)
		{
			auto itpair = m_Children.equal_range(event.ViewportFrame);
			for (auto it = itpair.first; it != itpair.second; ++it)
			{
				auto viewportFrame = std::dynamic_pointer_cast<GUI::Viewport>(it->second);
				if (!viewportFrame)
					continue;

				viewportFrame->SetCamera(event.CameraEntity);
			}
		}

		std::shared_ptr<Systems::TransformSystem> m_TransformSystem;

		void EnqueueModel(RenderQueue<ModelJob> &renderQueue, Model* model, glm::mat4 modelMatrix)
		{
			for (auto texGroup : model->TextureGroups)
			{
				ModelJob job;
				job.TextureID = texGroup.Texture->ResourceID;
				job.DiffuseTexture = *texGroup.Texture;
				job.NormalTexture = *texGroup.NormalMap;
				job.SpecularTexture = *texGroup.SpecularMap;
				job.VAO = model->VAO;
				job.StartIndex = texGroup.StartIndex;
				job.EndIndex = texGroup.EndIndex;
				job.ModelMatrix = modelMatrix;
					
				renderQueue.Add(job);
			}
		}

		void EnqueueSprite(RenderQueue<SpriteJob> &renderQueue, Texture* texture, glm::mat4 modelMatrix)
		{
			SpriteJob job;
			job.TextureID = texture->ResourceID;
			job.Texture = *texture;
			job.ModelMatrix = modelMatrix;
		}
	};

}

#endif // GUI_WorldFrame_h__
