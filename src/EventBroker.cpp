#include "PrecompiledHeader.h"
#include "EventBroker.h"

BaseEventRelay::~BaseEventRelay()
{
	m_Broker->Unsubscribe(*this);
}

void EventBroker::Unsubscribe(BaseEventRelay &relay) // ?
{
	auto itpair = m_Subscribers.equal_range(relay.m_TypeName);
	for (auto it = itpair.first; it != itpair.second; ++it)
	{
		if (it->second == &relay)
		{
			m_Subscribers.erase(it);
			break;
		}
	}
}

void EventBroker::Subscribe(BaseEventRelay &relay)
{
	relay.m_Broker = this;
	m_Subscribers.insert(std::make_pair(relay.m_TypeName, &relay));
}