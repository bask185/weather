// HEADER FILES
#include <Arduino.h>
#include "weather.h"
#include "src/basics/macros.h"
#include "src/basics/io.h"
#include "src/basics/stateMachineClass.h"

static StateMachine sm ;

// VARIABLES
static uint16_t redPwm, greenPwm, bluePwm, whitePwm;

struct
{
    uint16_t red ;
    uint16_t blue ;
    uint16_t green ;
    uint16_t white ;
    uint16_t coolWhite ;
} set, current ;

const int dayTime = 60000 ;         // time of each day section in ms
const int lightInterval = 100 ;

// FUNCTIONS
void updateLighting()
{
    if( set.red < current.red ) current.red -- ;
    if( set.red > current.red ) current.red ++ ;
    
    if( set.green < current.green ) current.green -- ;
    if( set.green > current.green ) current.green ++ ;
    
    if( set.blue < current.blue ) current.blue -- ;
    if( set.blue > current.blue ) current.blue ++ ;
    
    if( set.white < current.white ) current.white -- ;
    if( set.white > current.white ) current.white ++ ;
    
    if( set.coolWhite < current.coolWhite ) current.coolWhite -- ;
    if( set.coolWhite > current.coolWhite ) current.coolWhite ++ ;

    Serial.print(" red: ");         Serial.print(  current.red);
    Serial.print(" green: ");       Serial.print(  current.green);
    Serial.print(" blue: ");        Serial.print(  current.blue);
    Serial.print(" white: ");       Serial.print(  current.white);
    Serial.print(" coolWhite: ");   Serial.println(current.coolWhitePin);

    if( flash )
    {
        pwmDriver.setPWM(redPin,        0, 4095);
        pwmDriver.setPWM(greenPin,      0, 4095);
        pwmDriver.setPWM(bluePin,       0, 4095);
        pwmDriver.setPWM(whitePin,      0, 4095);
        pwmDriver.setPWM(coolWhitePin,  0, 4095);
    }
    else 
    {
        pwmDriver.setPWM(redPin,        0,   current.red   );
        pwmDriver.setPWM(redPin,        0,   current.green );
        pwmDriver.setPWM(bluePin,       0,   current.blue  );
        pwmDriver.setPWM(whitePin,      0,   current.white );
        pwmDriver.setPWM(coolWhitePin,  0,   current.coolWhite );
    }
}

extern void weatherInit(void)
{
    sm.nextState( morning, 0 ) ;
    // pwmDriver.setFrequency( 200 ) ;
    // pwmDriver.setClock(27000000) ;
}

// STATE FUNCTIONS
StateFunction( morning )
{
    if( sm.entryState() )
    {
        set.red         = 10 ;  // invullen naar believen, kleiner houden dan 4095 ( = 100% PWM)
        set.green       = 10 ;
        set.blue        = 10 ;
        set.white       = 10 ;
        set.coolWhite   = 10 ;
        
        sm.setTimeOut( dayTime ) ;
    }
    if( sm.onState() )
    {
        if( sm.timeOut() ) sm.exit() ;
    }
    if( sm.exitState() )
    {
        
    }
    return sm.endState() ;
}

StateFunction( afternoon )
{
    if( sm.entryState() )
    {
        set.red         = 0 ;
        set.green       = 0 ;
        set.blue        = 1600 ;
        set.white       = 1200 ;
        set.coolWhite   = 0 ;
        
        sm.setTimeOut( dayTime ) ;
    }
    if( sm.onState() )
    {
        
        if( sm.timeOut() ) sm.exit() ;
    }
    if( sm.exitState() )
    {
        
    }
    return sm.endState() ;
}

StateFunction( evenening )
{
    if( sm.entryState() )
    {
        set.red         = 0 ;  // avond rood ?
        set.green       = 0 ;
        set.blue        = 204 ;
        set.white       = 0 ;
        set.coolWhite   = 0 ;
        
        sm.setTimeOut( dayTime ) ;
    }
    if( sm.onState() )
    {
        
        if( sm.timeOut() ) sm.exit() ;
    }
    if( sm.exitState() )
    {
        
    }
    return sm.endState() ;
}

StateFunction( night )
{
    if( sm.entryState() )
    {
        set.red         = 0 ; 
        set.green       = 0 ;
        set.blue        = 204 ;  // zelfde als evening
        set.white       = 0 ;
        set.coolWhite   = 0 ;
        
        sm.setTimeOut( dayTime ) ;
    }
    if( sm.onState() )
    {
        
        if( sm.timeOut() ) sm.exit() ;
    }
    if( sm.exitState() )
    {
        
    }
    return sm.endState() ;
}

StateFunction( raining )
{
    if( sm.entryState() )
    {
        set.red         = 0 ;
        set.green       = 0 ;
        set.blue        = 800 ;
        set.white       = 600 ;
        set.coolWhite   = 0 ;
        
        sm.setTimeOut( dayTime ) ;
    }
    if( sm.onState() )
    {   
        // loop raining sounds
        
        if( sm.timeOut() ) sm.exit() ;
    }
    if( sm.exitState() )
    {
        
    }
    return sm.endState() ;
}

StateFunction( storm )
{
    if( sm.entryState() )
    {
        set.red         = 0 ;
        set.green       = 0 ;
        set.blue        = 800 ;
        set.white       = 600 ;
        set.coolWhite   = 0 ;
        
        sm.setTimeOut( dayTime ) ;
    }
    if( sm.onState() )
    {
        // do flashes and play thunder sound

        if( sm.timeOut() ) sm.exit() ;
    }
    if( sm.exitState() )
    {
        
    }
    return sm.endState() ;
}


// STATE MACHINE
extern uint8_t weather()
{
    REPEAT_MS( lightInterval ) ;
    updateLighting() ;
    END_REPEAT
    
    STATE_MACHINE_BEGIN

    State(morning) {
        uint8_t val = random(3);
        if( val != 1 ) sm.nextState( afternoon, 0 ) ; // chance 75% it won't rain
        else           sm.nextState( raining,   0 ) ; }

    State(afternoon) {
        uint8_t val = random(2);
        if( val != 1 ) sm.nextState( evenening, 0 ) ; // chance 66% it won't storm
        else           sm.nextState( storm,     0 ) ; }

    State(evenening) {
        sm.nextState( night, 0 ) ; }

    State(night) {
        sm.nextState( morning, 0 ) ; }

    State(raining) {
        uint8_t val = random(2);
        if( val != 1 ) sm.nextState( evenening, 0 ) ; // chance 66% it won't storm
        else           sm.nextState( storm,     0 ) ; }

    State(storm) {
        sm.nextState( night, 0 ) ; }

    STATE_MACHINE_END
}
