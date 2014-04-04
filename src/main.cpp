#include "PrecompiledHeader.h"
#include "Engine.h"

int main(int argc, char* argv[])
{
	Engine engine(argc, argv);
	while (engine.Running())
		engine.Tick();

	return 0;
}