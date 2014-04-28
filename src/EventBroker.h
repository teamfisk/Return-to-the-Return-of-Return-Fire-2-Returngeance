#ifndef MessageRelay_h__
#define MessageRelay_h__

#include <typeinfo>
#include <functional>
#include <unordered_map>
#include <list>

struct Event
{
protected:
	Event() { }
};

class EventBroker;

class BaseEventRelay
{
friend class EventBroker;

protected:
	BaseEventRelay(std::string typeName)
		: m_TypeName(typeName) { }
	~BaseEventRelay();

public:
	virtual bool Receive(const Event &event) = 0;

protected:
	std::string m_TypeName;
	EventBroker* m_Broker;
};

template <typename EventType>
class EventRelay : public BaseEventRelay
{
public:
	typedef std::function<bool(const EventType&)> CallbackType;

	EventRelay()
		: m_Callback(nullptr)
		, BaseEventRelay(typeid(EventType).name()) { }
	EventRelay(CallbackType callback) 
		: m_Callback(callback)
		, BaseEventRelay(typeid(EventType).name()) { }

protected:
	bool Receive(const Event &event) override;

private:
	CallbackType m_Callback;
};

template <typename EventType>
bool EventRelay<EventType>::Receive(const Event &event)
{
	if (m_Callback != nullptr)
	{
		return m_Callback(static_cast<const EventType&>(event));
	}
	else
	{
		return false;
	}
}

class EventBroker
{
template <typename EventType> friend class EventRelay;

public:
	template <typename EventType>
	void Publish(const EventType &event);
	void Subscribe(BaseEventRelay &relay);
	void Unsubscribe(BaseEventRelay &relay);

private:
	std::unordered_multimap<std::string, BaseEventRelay*> m_Subscribers;
};


template <typename EventType>
void EventBroker::Publish(const EventType &event)
{
	auto itpair = m_Subscribers.equal_range(typeid(EventType).name());
	for (auto it = itpair.first; it != itpair.second; ++it)
	{
		it->second->Receive(event);
	}
}

#endif // MessageRelay_h__
