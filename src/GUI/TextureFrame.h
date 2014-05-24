#ifndef GUI_TextureFrame_h__
#define GUI_TextureFrame_h__

#include "GUI/Frame.h"
#include "Texture.h"

namespace GUI
{

class TextureFrame : public Frame
{
public:
	TextureFrame(std::shared_ptr<Frame> parent, std::string name)
		: Frame(parent, name) { }

	void Update(double dt) override
	{
		Frame::Update(dt);
	}

	void Draw(std::shared_ptr<Renderer> renderer) override
	{
		if (m_Texture == nullptr)
			return;

		RenderQueue<SpriteJob> rq;

		auto textureAsset = ResourceManager->Load<Texture>("Texture", m_Texture);

		SpriteJob job;

		Components::Transform absoluteTransform = m_TransformSystem->AbsoluteTransform(entity);
		glm::quat orientation2D = glm::angleAxis(glm::eulerAngles(absoluteTransform->Orientation).z, glm::vec3(0, 0, -1));
		glm::mat4 modelMatrix = glm::translate(absoluteTransform.Position);
		*glm::toMat4(orientation2D)
			* glm::scale(absoluteTransform.Scale);
		EnqueueSprite(spriteQueue, textureAsset, modelMatrix);
	}

	std::shared_ptr<::Texture> Texture() const { return m_Texture; }
	void SetTexture(std::string resourceName)
	{
		m_Texture = std::shared_ptr<::Texture>(ResourceManager->Load<::Texture>("Texture", resourceName));
	}

protected:
	std::shared_ptr<::Texture> m_Texture;
};

}

#endif // GUI_TextureFrame_h__
