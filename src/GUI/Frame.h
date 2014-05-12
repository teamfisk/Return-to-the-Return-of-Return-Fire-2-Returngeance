#ifndef GUI_Frame_h__
#define GUI_Frame_h__

#include <memory>

#include "Util/Rectangle.h"
#include "EventBroker.h"

// HACK: Decouple renderer plz
#include "Renderer.h"

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
	Frame(std::shared_ptr<::EventBroker> eventBroker) 
		: EventBroker(eventBroker)
		, Rectangle() 
	{ Initialize(); }
	// Create a frame as a child
	Frame(std::shared_ptr<Frame> parent)
		: Rectangle(static_cast<Rectangle>(*parent)) // Clone parent rectangle using copy constructor
	{ SetParent(parent); Initialize(); }

	virtual void Initialize() { }

	std::shared_ptr<Frame> Parent() const { return m_Parent; }
	void SetParent(std::shared_ptr<Frame> parent) 
	{ 
		parent->AddChild(std::shared_ptr<Frame>(this));
		m_Parent = parent;
		EventBroker = parent->EventBroker;
	}

	void AddChild(std::shared_ptr<Frame> child)
	{
		m_Children.push_back(child);
		if (m_Parent != nullptr)
		{
			m_Parent->AddChild(child);
		}
	}

	typedef std::list<std::shared_ptr<Frame>>::const_iterator FrameChildrenIterator;
	FrameChildrenIterator begin()
	{
		return m_Children.begin();
	}
	FrameChildrenIterator end()
	{
		return m_Children.end();
	}

	virtual void Update(double dt) { }
	virtual void Draw(Renderer* renderer) { }

protected:
	std::shared_ptr<::EventBroker> EventBroker;
	std::shared_ptr<Frame> m_Parent;
	std::list<std::shared_ptr<Frame>> m_Children;
};

}

#endif // GUI_Frame_h__
