#include "PrecompiledHeader.h"
#include "EventBroker.h"
#include "Events/BindKey.h"

BaseEventRelay::~BaseEventRelay()
{
	if (m_Broker != nullptr)
	{
		m_Broker->Unsubscribe(*this);
	}
}

void EventBroker::Unsubscribe(BaseEventRelay &relay) // ?
{
	/*auto itpair = m_Subscribers.equal_range(relay.m_EventTypeName);
	for (auto it = itpair.first; it != itpair.second; ++it)
	{
	if (it->second == &relay)
	{
	m_Subscribers.erase(it);
	break;
	}
	}*/
}

void EventBroker::Subscribe(BaseEventRelay &relay)
{
	relay.m_Broker = this;
	m_ContextSubscribers[relay.m_ContextTypeName][relay.m_EventTypeName] = &relay;
}

int EventBroker::Process(std::string contextTypeName)
{
	auto it = m_ContextSubscribers.find(contextTypeName);
	if (it == m_ContextSubscribers.end())
		return 0;

	int eventsProcessed = 0;

	EventRelays_t &relays = it->second;
	for (auto &pair : *m_EventQueueRead)
	{
		std::string &eventTypeName = pair.first;
		std::shared_ptr<Event> event = pair.second;

		/*if (eventTypeName == "struct Events::BindKey")
		{
			auto bindKey = static_cast<Events::BindKey*>(event.get());
			LOG_DEBUG("HsssEJ");
		}*/

		auto it2 = relays.find(eventTypeName);
		if (it2 == relays.end())
			continue;

		auto relay = it2->second;
		relay->Receive(event);
		eventsProcessed++;
	}

	return eventsProcessed;
}

void EventBroker::Clear()
{
	std::swap(m_EventQueueRead, m_EventQueueWrite);
	m_EventQueueWrite->clear();
}
