#include <iostream>

#include "Leap.h"

void Demo()
{
	
}

int main()
{
	leap::LeapEngine engine{};
	engine.Run(Demo, 60);
	return 0;
}