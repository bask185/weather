enum weatherStates
{
    weatherIDLE,
    morning,
    afternoon,
    evenening,
    night,
    raining,
    storm
} ;

extern uint8_t weather(void) ; 
extern void weatherInit(void) ; 
