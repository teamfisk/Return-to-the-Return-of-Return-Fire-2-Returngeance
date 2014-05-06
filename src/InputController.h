#ifndef InputController_h__
#define InputController_h__

#include <memory>

#include "EventBroker.h"
#include "Events/InputCommand.h"
#include "Events/MouseMove.h"

class InputController
{
public:
	InputController(std::shared_ptr<::EventBroker> eventBroker)
		: EventBroker(eventBroker) { Initialize(); }

	virtual void Initialize()
	{
		EVENT_SUBSCRIBE_MEMBER(m_EInputCommand, &InputController::OnCommand);
		EVENT_SUBSCRIBE_MEMBER(m_EMouseMove, &InputController::OnMouseMove);
	}

	virtual bool OnCommand(const Events::InputCommand &event) { return false; }
	virtual bool OnMouseMove(const Events::MouseMove &event) { return false; }

protected:
	std::shared_ptr<::EventBroker> EventBroker;

private:
	EventRelay<Events::InputCommand> m_EInputCommand;
	EventRelay<Events::MouseMove> m_EMouseMove;
};

#endif // InputController_h__
