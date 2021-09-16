#include "src/basics/io.h"
#include "roundRobinTasks.h"
#include "weather.h"

void setup()
{
	initIO();
	Serial.begin(115200);
	weatherInit();
}

void loop()
{
	processRoundRobinTasks();

	weather();
}