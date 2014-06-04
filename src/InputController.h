#ifndef InputController_h__
#define InputController_h__

#include <memory>

#include "EventBroker.h"
#include "Events/InputCommand.h"
#include "Events/MouseMove.h"
#include "Events/GameStart.h"
#include "Events/GameOver.h"

template <typename EventContext>
class InputController
{
public:
	InputController(std::shared_ptr<::EventBroker> eventBroker)
		: EventBroker(eventBroker)
		, InGame(true)
	{ Initialize(); }

	virtual void Initialize()
	{
		EVENT_SUBSCRIBE_MEMBER(m_EInputCommand, &InputController::_OnCommand);
		EVENT_SUBSCRIBE_MEMBER(m_EMouseMove, &InputController::_OnMouseMove);
		EVENT_SUBSCRIBE_MEMBER(m_EGameStart, &InputController::OnGameStart);
		EVENT_SUBSCRIBE_MEMBER(m_EGameOver, &InputController::OnGameOver);
	}

	virtual bool OnCommand(const Events::InputCommand &event) { return false; }
	virtual bool OnMouseMove(const Events::MouseMove &event) { return false; }
	virtual bool OnGameStart(const Events::GameStart &event)
	{
		InGame = true;
		return true;
	}
	virtual bool OnGameOver(const Events::GameOver &event) 
	{ 
		InGame = false;
		return true; 
	}

protected:
	std::shared_ptr<::EventBroker> EventBroker;
	bool InGame;

private:
	EventRelay<EventContext, Events::InputCommand> m_EInputCommand;
	EventRelay<EventContext, Events::MouseMove> m_EMouseMove;
	EventRelay<EventContext, Events::GameStart> m_EGameStart;
	EventRelay<EventContext, Events::GameOver> m_EGameOver;

	virtual bool _OnCommand(const Events::InputCommand &event) 
	{ 
		if (InGame || event.Value == 0)
			return OnCommand(event);
		else
			return false;
	}
	virtual bool _OnMouseMove(const Events::MouseMove &event) 
	{ 
		if (InGame)
			return OnMouseMove(event);
		else
			return false; 
	}
};

#endif // InputController_h__
