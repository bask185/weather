#include <Arduino.h>
#include "io.h"
extern void initIO(void) {
	pinMode(dirPin, OUTPUT);
	pinMode(stepPin, OUTPUT);
	pinMode(enablePin, OUTPUT);
	pinMode(speedPin, INPUT);
	pinMode(cwPin, INPUT_PULLUP);
	pinMode(ccwPin, INPUT_PULLUP);
	pinMode(modePin, INPUT_PULLUP);
	pinMode(recordPin, INPUT_PULLUP);
	pinMode(SensorPin, INPUT_PULLUP);
}