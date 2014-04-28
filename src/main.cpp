#include "PrecompiledHeader.h"
#include "Engine.h"
#include "EventBroker.h"

struct TestEvent : Event
{
	std::string Hello;
};

bool OnTestEvent(const TestEvent &event)
{
	std::cout << "TestEvent.Hello = " << event.Hello << std::endl;
	return true;
}

int main(int argc, char* argv[])
{
	EventBroker broker;

	TestEvent e;
	e.Hello = "Hello world";

	{
		EventRelay<TestEvent> testRelay = &OnTestEvent;
		broker.Subscribe(testRelay);

		broker.Publish<TestEvent>(e);
	}

	broker.Publish<TestEvent>(e);

	return 0;

	Engine engine(argc, argv);
	while (engine.Running())
		engine.Tick();

	return 0;
}
