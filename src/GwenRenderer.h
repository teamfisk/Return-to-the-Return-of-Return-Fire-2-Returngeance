#ifndef GwenRenderer_h__
#define GwenRenderer_h__

#include <Gwen/Gwen.h>
#include <Gwen/BaseRender.h>

#include "Renderer.h"

class GwenRenderer : public Gwen::Renderer::Base
{
public:
	GwenRenderer(Renderer* renderer)
		: m_Renderer(renderer) { }

	void Begin() override;
	void End() override;

	void SetDrawColor(Gwen::Color color) override;
	void DrawFilledRect(Gwen::Rect rect) override;

	void StartClip() override;
	void EndClip() override;

	void DrawTexturedRect(Gwen::Texture* pTexture, Gwen::Rect pTargetRect, float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f) override;
	void LoadTexture(Gwen::Texture* pTexture) override;
	void FreeTexture(Gwen::Texture* pTexture) override;
	Gwen::Color PixelColour(Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color & col_default) override;

	void LoadFont(Gwen::Font* pFont) override;
	void FreeFont(Gwen::Font* pFont) override;
	void RenderText(Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString & text) override;
	Gwen::Point MeasureText(Gwen::Font* pFont, const Gwen::UnicodeString & text) override;

private:
	Renderer* m_Renderer;

	Gwen::Color m_Color;
};
#endif // GwenRenderer_h__