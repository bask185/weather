enum weatherStates
{
    weatherIDLE,
    morning,
    afternoon,
    evening,
    night,
    raining,
    storm
} ;

extern uint8_t weather(void) ; 
extern void weatherInit(void) ; 
