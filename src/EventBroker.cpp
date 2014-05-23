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
	auto contextIt = m_ContextRelays.find(relay.m_ContextTypeName);
	if (contextIt == m_ContextRelays.end())
		return;

	auto eventRelays = contextIt->second;

	auto itpair = eventRelays.equal_range(relay.m_EventTypeName);
	for (auto it = itpair.first; it != itpair.second; ++it)
	{
		if (it->second == &relay)
		{
			eventRelays.erase(it);
			break;
		}
	}
}

void EventBroker::Subscribe(BaseEventRelay &relay)
{
	relay.m_Broker = this;
	m_ContextRelays[relay.m_ContextTypeName].insert(std::make_pair(relay.m_EventTypeName, &relay));
}

int EventBroker::Process(std::string contextTypeName)
{
	auto it = m_ContextRelays.find(contextTypeName);
	if (it == m_ContextRelays.end())
		return 0;

	EventRelays_t &relays = it->second;

	int eventsProcessed = 0;
	for (auto &pair : *m_EventQueueRead)
	{
		std::string &eventTypeName = pair.first;
		std::shared_ptr<Event> event = pair.second;

		auto itpair = relays.equal_range(eventTypeName);
		for (auto it2 = itpair.first; it2 != itpair.second; ++it2)
		{
			auto relay = it2->second;
			relay->Receive(event);
			eventsProcessed++;
		}
	}

	return eventsProcessed;
}

void EventBroker::Clear()
{
	std::swap(m_EventQueueRead, m_EventQueueWrite);
	m_EventQueueWrite->clear();
}
