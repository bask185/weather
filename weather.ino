#include "weather.h"

void setup()
{
	Serial.begin(115200);
	weatherInit();
}

void loop()
{
	weather();
}