#include "weather.h"
#include "src/NmraDcc.h"

NmraDcc  Dcc ;
DCC_MSG  Packet ;

// Define the Arduino input Pin number for the DCC Signal 
#define DCC_PIN     2

void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
{
	Serial.print("notifyDccAccTurnoutOutput: ") ;
	Serial.print(Addr,DEC) ;
	Serial.print(',');
	Serial.print(Direction,DEC) ;
	Serial.print(',');
	Serial.println(OutputPower, HEX) ;
}


void setup()
{
	Serial.begin(115200);
	weatherInit();
}

void loop()
{
	weather();
}