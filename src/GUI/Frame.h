#ifndef GUI_Frame_h__
#define GUI_Frame_h__

#include <memory>

#include "Util/Rectangle.h"
#include "EventBroker.h"

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
		m_Parent = parent;
		EventBroker = parent->EventBroker;
	}
	virtual void Update(double dt) { }

protected:
	std::shared_ptr<::EventBroker> EventBroker;
	std::shared_ptr<Frame> m_Parent;
};

}

#endif // GUI_Frame_h__
