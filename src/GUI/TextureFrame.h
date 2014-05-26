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

	void Draw(std::shared_ptr<Renderer> renderer) override
	{
		if (m_Texture == nullptr)
			return;

		RenderQueue.Clear();
		SpriteJob job;
		job.TextureID = m_Texture->ResourceID;
		job.Texture = *m_Texture;
		RenderQueue.Add(job);

		renderer->SetCamera(nullptr);
		renderer->Draw(RenderQueue);
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
