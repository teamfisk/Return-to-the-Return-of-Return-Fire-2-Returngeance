#include "PrecompiledHeader.h"
#include "Engine.h"

int main(int argc, char* argv[])
{
	Engine engine(argc, argv);
	LOG_INFO("------------ Engine initialized ------------");
	while (engine.Running())
		engine.Tick();

	return 0;
}
