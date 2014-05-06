#ifndef InputController_h__
#define InputController_h__

#include <memory>

#include "EventBroker.h"
#include "Events/InputCommand.h"

class InputController
{
public:
	InputController(std::shared_ptr<::EventBroker> eventBroker)
		: EventBroker(EventBroker) { Initialize(); }

	virtual void Initialize()
	{
		EVENT_SUBSCRIBE_MEMBER(m_EInputCommand, &InputController::OnCommand)
	}

	virtual bool OnCommand(const Events::InputCommand &event) { }

protected:
	std::shared_ptr<::EventBroker> EventBroker;

private:
	EventRelay<Events::InputCommand> m_EInputCommand;
	
};

#endif // InputController_h__
