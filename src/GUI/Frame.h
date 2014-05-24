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
	{ Initialize(); }
	// Create a frame as a child
	Frame(std::shared_ptr<Frame> parent, std::string name)
		: Rectangle(static_cast<Rectangle>(*parent)) // Clone parent rectangle using copy constructor
		, m_Name(name)
	{ SetParent(parent); Initialize(); }

	virtual void Initialize() { }
	std::shared_ptr<Frame> Parent() const { return m_Parent; }
	void SetParent(std::shared_ptr<Frame> parent) 
	{ 
		parent->AddChild(std::shared_ptr<Frame>(this));
		m_Parent = parent;
		EventBroker = parent->EventBroker;
		ResourceManager = parent->ResourceManager;
	}

	void AddChild(std::shared_ptr<Frame> child)
	{
		m_Children.push_back(std::make_pair(child->Name(), child));
	}

	typedef std::map<std::string, std::shared_ptr<Frame>>::const_iterator FrameChildrenIterator;
	FrameChildrenIterator begin()
	{
		return m_Children.begin();
	}
	FrameChildrenIterator end()
	{
		return m_Children.end();
	}

	std::string Name() const { return m_Name; }
	void SetName(std::string val) { m_Name = val; }

	virtual void Update(double dt)
	{
		for (auto &pair : m_Children)
		{
			pair.second->Update(dt);
		}
	}

	void DrawLayered(std::shared_ptr<Renderer> renderer)
	{
		renderer->SetViewport(GetLeft(), GetTop(), GetRight(), GetBottom());
		this->Draw(renderer);
		renderer->Draw();

		for (auto &pair : m_Children)
		{
			pair.second->Draw(renderer);
		}
	}

	virtual void Draw(std::shared_ptr<Renderer> renderer)
	{
		renderer->SetViewport(GetLeft(), GetTop(), GetRight(), GetBottom());
		renderer->Draw();

		
	}

protected:
	std::shared_ptr<::EventBroker> EventBroker;
	std::shared_ptr<::ResourceManager> ResourceManager;
	std::string m_Name;
	
	std::shared_ptr<Frame> m_Parent;
	std::multimap<std::string, std::shared_ptr<Frame>> m_Children; // name -> frame

};

}

#endif // GUI_Frame_h__
