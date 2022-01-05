// HEADER FILES
#include <Arduino.h>
#include "weather.h"
#include "src/macros.h"
#include "src/stateMachineClass.h"
#include "src/Adafruit_PWMServoDriver.h"

static StateMachine sm ;

Adafruit_PWMServoDriver pwmDriver( 0x40 ) ;

// VARIABLES
static uint16_t redPwm, greenPwm, bluePwm, whitePwm;
bool flash ;

struct
{
    uint16_t red ;
    uint16_t blue ;
    uint16_t green ;
    uint16_t white ;
    uint16_t coolWhite ;
} set, current ;

const int dayTime = 5000 ;         // time of each day section in ms
const int lightInterval = 500 ;


const int redPin        = 0 ;   // pins on PCA
const int greenPin      = 15 ;
const int bluePin       = 12 ;
const int whitePin      = 13 ;
const int coolWhitePin  = 14 ;

// FUNCTIONS
void updateLighting()
{
REPEAT_MS( lightInterval ) ;
    if( set.red < current.red )             current.red -- ;
    if( set.red > current.red )             current.red ++ ;
    
    if( set.green < current.green )         current.green -- ;
    if( set.green > current.green )         current.green ++ ;
    
    if( set.blue < current.blue )           current.blue -- ;
    if( set.blue > current.blue )           current.blue ++ ;
    
    if( set.white < current.white )         current.white -- ;
    if( set.white > current.white )         current.white ++ ;
    
    if( set.coolWhite < current.coolWhite ) current.coolWhite -- ;
    if( set.coolWhite > current.coolWhite ) current.coolWhite ++ ;

    Serial.print(" red: ");         Serial.print(  current.red);
    Serial.print(" green: ");       Serial.print(  current.green);
    Serial.print(" blue: ");        Serial.print(  current.blue);
    Serial.print(" white: ");       Serial.print(  current.white);
    Serial.print(" coolWhite: ");   Serial.println(current.coolWhite);

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
END_REPEAT
}

extern void weatherInit(void)
{
    sm.nextState( morning, 0 ) ;    // begin at morning

    pwmDriver.begin() ;
    pwmDriver.setOscillatorFrequency(27000000) ;
    pwmDriver.setPWMFreq(200) ;
}

// STATE FUNCTIONS
StateFunction( morning )
{
    if( sm.entryState() )
    {
        Serial.println("morning begins") ;
        
        // set.red         = 600 ;  // invullen naar believen, kleiner houden dan 4095 ( = 100% PWM)
        // set.green       = 0 ;
        // set.blue        = 600 ;
        // set.white       = 1000 ;
        // set.coolWhite   = 0 ;

        set.red         = 4095 ;  // invullen naar believen, kleiner houden dan 4095 ( = 100% PWM)
        set.green       = 4095 ;
        set.blue        = 4095 ;
        set.white       = 4095 ;
        set.coolWhite   = 0 ;
        
        sm.setTimeout( dayTime ) ;
    }
    
    if( sm.onState() )
    {
        //if( sm.timeout() ) sm.exit() ;
    }
    
    if( sm.exitState() )
    {
        Serial.println("morning has ended") ;
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
        
        sm.setTimeout( dayTime ) ;
    }

    if( sm.onState() )
    {
        
        if( sm.timeout() ) sm.exit() ;
    }

    if( sm.exitState() )
    {
        
    }
    return sm.endState() ;
}

StateFunction( evening )
{
    if( sm.entryState() )
    {
        set.red         = 0 ;  // avond rood ?
        set.green       = 0 ;
        set.blue        = 204 ;
        set.white       = 0 ;
        set.coolWhite   = 0 ;
        
        sm.setTimeout( dayTime ) ;
    }

    if( sm.onState() )
    {
        
        if( sm.timeout() ) sm.exit() ;
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
        
        sm.setTimeout( dayTime ) ;
    }

    if( sm.onState() )
    {
        
        if( sm.timeout() ) sm.exit() ;
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
        
        sm.setTimeout( dayTime ) ;
    }

    if( sm.onState() )
    {   
        // loop raining sounds
        
        if( sm.timeout() ) sm.exit() ;
    }

    if( sm.exitState() )
    {
        
    }
    return sm.endState() ;
}

StateFunction( storm )
{
    static uint32_t interval ;
    static uint8_t lightningState ;
    
    if( sm.entryState() )
    {
        set.red         = 0 ;
        set.green       = 0 ;
        set.blue        = 800 ;
        set.white       = 600 ;
        set.coolWhite   = 0 ;
        
        sm.setTimeout( dayTime ) ;
        flash = false ;
        
        interval = 0 ;
        lightningState = 0 ;
    }

    if( sm.onState() )
    {
        REPEAT_MS( interval )
        switch( lightningState )
        {
        case 0 : // start a flash
            flash = true ;
            interval = random( 50, 100 ) ;      // flash lasts between 50 and 100ms
            lightningState = 1 ;
            break ;
        
        case 1: // stop the flash
            flash = false ;
            interval = random( 100, 2000 ) ;  // wait between 100 and 2000ms to start thunder sound
            lightningState = 2 ;
            break ;

        case 2: // make thunder sound
            interval = random( 8000, 15000 ) ;      // new flash between 8 and 15 seconds
            // mp3.play("thunder.mp3" );            // to be added
            lightningState = 0 ;
            break ;
        }
        END_REPEAT

        if( sm.timeout() ) sm.exit() ;
    }
    
    if( sm.exitState() )
    {
        flash = false ;
    }
    
    return sm.endState() ;
}


// STATE MACHINE
extern uint8_t weather()
{
    updateLighting() ;
    
    STATE_MACHINE_BEGIN

    State(morning) {
        uint8_t val = random(3);
        if( val != 1 ) sm.nextState( afternoon, 0 ) ; // chance 75% it won't rain
        else           sm.nextState( raining,   0 ) ; }

    State(afternoon) {
        uint8_t val = random(2);
        if( val != 1 ) sm.nextState( evening, 0 ) ; // chance 66% it won't storm
        else           sm.nextState( storm,     0 ) ; }

    State(evening) {
        sm.nextState( night, 0 ) ; }

    State(night) {
        sm.nextState( morning, 0 ) ; }

    State(raining) {
        uint8_t val = random(2);
        if( val != 1 ) sm.nextState( evening, 0 ) ; // chance 66% it won't storm
        else           sm.nextState( storm,     0 ) ; }

    State(storm) {
        sm.nextState( night, 0 ) ; }

    STATE_MACHINE_END
}
