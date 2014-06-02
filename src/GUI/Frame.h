#ifndef GUI_Frame_h__
#define GUI_Frame_h__

#include <memory>
#include <map>

#include "Util/Rectangle.h"
#include "EventBroker.h"
#include "Events/KeyDown.h"
#include "Events/KeyUp.h"
#include "Events/InputCommand.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "RenderQueue.h"
#include "Texture.h"

namespace GUI
{
	
class Frame : public Rectangle
{
public:
	enum class Anchor
	{
		Left,
		Right,
		Top,
		Bottom
	};

	static const int BaseWidth = 1280;
	static const int BaseHeight = 720;

	// Set up a base frame with an event broker
	Frame(std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
		: EventBroker(eventBroker)
		, ResourceManager(resourceManager)
		, Rectangle()
		, m_Name("UIParent")
		, m_Layer(0)
		, m_Hidden(false)
	{ Initialize(); }

	// Create a frame as a child
	Frame(Frame* parent, std::string name)
		: m_Name(name)
		, m_Layer(0)
		, m_Hidden(false)
	{ SetParent(std::shared_ptr<Frame>(parent)); Initialize(); }

	::RenderQueue RenderQueue;

	std::shared_ptr<Frame> Parent() const { return m_Parent; }
	void SetParent(std::shared_ptr<Frame> parent) 
	{
		if (parent == nullptr)
		{
			LOG_ERROR("Failed to create frame \"%s\": Invalid parent", m_Name.c_str());
			return;
		}

		Width = parent->Width;
		Height = parent->Height;
		m_Layer = parent->Layer() + 1;
		parent->AddChild(std::shared_ptr<Frame>(this));
		m_Parent = parent;
		EventBroker = parent->EventBroker;
		ResourceManager = parent->ResourceManager;
	}

	void AddChild(std::shared_ptr<Frame> child)
	{
		m_Children[child->m_Layer].insert(std::make_pair(child->Name(), child));
		if (m_Parent)
		{
			m_Parent->AddChild(child);
		}
	}

	typedef std::map<std::string, std::shared_ptr<Frame>>::const_iterator FrameChildrenIterator;

	std::string Name() const { return m_Name; }
	void SetName(std::string val) { m_Name = val; }
	int Layer() const { return m_Layer; }
	bool Hidden() const 
	{ 
		if (m_Parent)
			return m_Parent->Hidden() || m_Hidden;
		else
			return m_Hidden; 
	}
	void Hide() { m_Hidden = true; }
	void Show() { m_Hidden = false; }

	int Left() const override
	{ 
		if (m_Parent)
			return m_Parent->Left() + X;
		else
			return X; 
	}
	int Right() const override
	{ 
		if (m_Parent)
			return std::min(m_Parent->Right(), Left() + Width);
		else
			return Left() + Width;
	}
	int Top() const override
	{ 
		if (m_Parent)
			return m_Parent->Top() + Y;
		else
			return Y;
	}
	int Bottom() const override 
	{
		if (m_Parent)
			return std::min(m_Parent->Bottom(), Top() + Height);
		else
			return Top() + Height;
	}

	glm::vec2 Scale()
	{
		if (m_Parent)
			return m_Parent->Scale();
		else
			return glm::vec2(Width, Height) / glm::vec2(BaseWidth, BaseHeight);
	}

	Rectangle AbsoluteRectangle()
	{
		int left = Left();
		int top = Top();
		int width = Right() - left;
		int height = Bottom() - top;
		return Rectangle(left, top, width, height);
	}

	void UpdateLayered(double dt)
	{
		if (this->Hidden())
			return;

		// Update ourselves
		this->Update(dt);

		// Update children
		for (auto &pairLayer : m_Children)
		{
			auto children = pairLayer.second;
			for (auto &pairChild : children)
			{
				auto child = pairChild.second;
				if (child->Hidden())
					continue;
				child->Update(dt);
			}
		}
	}
	virtual void Update(double dt) { }

	void DrawLayered(std::shared_ptr<Renderer> renderer)
	{
		if (this->Hidden())
			return;

		// Draw ourselves
		renderer->SetScissor(AbsoluteRectangle());
		this->Draw(renderer);

		// Draw children
		for (auto &pairLayer : m_Children)
		{
			auto children = pairLayer.second;
			for (auto &pairChild : children)
			{
				auto child = pairChild.second;
				if (child->Hidden())
					continue;
				Rectangle rect = child->AbsoluteRectangle();
				renderer->SetScissor(rect);
				child->Draw(renderer);
			}
		}
	}

	virtual void Draw(std::shared_ptr<Renderer> renderer) { }

protected:
	std::shared_ptr<::EventBroker> EventBroker;
	std::shared_ptr<::ResourceManager> ResourceManager;

	std::string m_Name;
	int m_Layer;
	bool m_Hidden;

	std::shared_ptr<Frame> m_Parent;
	typedef std::multimap<std::string, std::shared_ptr<Frame>> Children_t; // name -> frame
	std::map<int, Children_t> m_Children; // layer -> Children_t

	virtual bool OnKeyDown(const Events::KeyDown &event) { return false; }
	virtual bool OnKeyUp(const Events::KeyUp &event) { return false; }
	//virtual bool OnMouseDown(const Events::KeyDown &event) { }
	//virtual bool OnMouseUp(const Events::KeyDown &event) { }
	virtual bool OnCommand(const Events::InputCommand &event) { return false; }

private:
	EventRelay<Frame, Events::KeyDown> m_EKeyDown;
	EventRelay<Frame, Events::KeyUp> m_EKeyUp;
	EventRelay<Frame, Events::InputCommand> m_EInputCommand;

	void Initialize()
	{
		EVENT_SUBSCRIBE_MEMBER(m_EKeyDown, &Frame::OnKeyDown);
		EVENT_SUBSCRIBE_MEMBER(m_EKeyUp, &Frame::OnKeyUp);
		EVENT_SUBSCRIBE_MEMBER(m_EInputCommand, &Frame::OnCommand);
	}
};

}

#endif // GUI_Frame_h__
