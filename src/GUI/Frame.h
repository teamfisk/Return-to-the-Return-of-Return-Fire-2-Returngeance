#ifndef GUI_Frame_h__
#define GUI_Frame_h__

#include <memory>
#include <map>

#include "Util/Rectangle.h"
#include "EventBroker.h"
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

	// Set up a base frame with an event broker
	Frame(std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
		: EventBroker(eventBroker)
		, ResourceManager(resourceManager)
		, Rectangle()
		, m_Name("UIParent")
		, m_Layer(0)
	{ }

	// Create a frame as a child
	Frame(Frame* parent, std::string name)
		: Rectangle(static_cast<Rectangle>(*parent)) // Clone parent rectangle using copy constructor
		, m_Name(name)
		, m_Layer(0)
	{ SetParent(std::shared_ptr<Frame>(parent)); }


	::RenderQueue RenderQueue;

	std::shared_ptr<Frame> Parent() const { return m_Parent; }
	void SetParent(std::shared_ptr<Frame> parent) 
	{
		if (parent == nullptr)
		{
			LOG_ERROR("Failed to create frame \"%s\": Invalid parent", m_Name.c_str());
			return;
		}

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
			return m_Parent->Right() + X + Width;
		else
			return X + Width; 
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
			return m_Parent->Bottom() + Y + Height;
		else
			return Y + Height;
	}

	Rectangle AbsoluteRectangle()
	{
		return Rectangle(Left(), Right(), Width, Height);
	}

	virtual void Update(double dt)
	{
		for (auto &pair : m_Children[m_Layer + 1])
		{
			pair.second->Update(dt);
		}
	}

	void DrawLayered(std::shared_ptr<Renderer> renderer)
	{
		// Draw ourselves
		renderer->SetViewport(AbsoluteRectangle());
		this->Draw(renderer);

		// Draw children
		for (auto &pairLayer : m_Children)
		{
			auto children = pairLayer.second;
			for (auto &pairChild : children)
			{
				auto child = pairChild.second;
				renderer->SetViewport(child->AbsoluteRectangle());
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
	
	std::shared_ptr<Frame> m_Parent;
	typedef std::multimap<std::string, std::shared_ptr<Frame>> Children_t; // name -> frame
	std::map<int, Children_t> m_Children; // layer -> Children_t

};

}

#endif // GUI_Frame_h__
