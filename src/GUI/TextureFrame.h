#ifndef GUI_TextureFrame_h__
#define GUI_TextureFrame_h__

#include "GUI/Frame.h"
#include "Texture.h"

namespace GUI
{

class TextureFrame : public Frame
{
public:
	TextureFrame(Frame* parent, std::string name)
		: Frame(parent, name)
		, m_Texture(nullptr)
		, m_Color(glm::vec4(1.f, 1.f, 1.f, 1.f))
	{ }

	void Draw(std::shared_ptr<Renderer> renderer) override
	{
		if (m_Texture == nullptr)
			return;

		RenderQueue.Clear();
		SpriteJob job;
		job.TextureID = m_Texture->ResourceID;
		job.Texture = *m_Texture;
		job.Color = m_Color;
		RenderQueue.Forward.Add(job);

		renderer->SetCamera(nullptr);
		renderer->DrawFrame(RenderQueue);
	}

	::Texture* Texture() const { return m_Texture; }
	void SetTexture(std::string resourceName)
	{
		m_Texture = ResourceManager->Load<::Texture>("Texture", resourceName);
	}

	glm::vec4 Color() const { return m_Color; }
	void SetColor(glm::vec4 val) { m_Color = val; }

protected:
	::Texture* m_Texture;
	glm::vec4 m_Color;
	
};

}

#endif // GUI_TextureFrame_h__
