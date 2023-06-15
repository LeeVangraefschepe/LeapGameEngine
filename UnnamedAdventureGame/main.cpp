#include <iostream>

#include "Leap.h"

int main()
{
	std::cout << "Hello World!\n";

	leap::LeapEngine engine{};
	engine.Run();

	return 0;
}