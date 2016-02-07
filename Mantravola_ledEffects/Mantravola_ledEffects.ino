/* Mantravola exp: led effects :
 use FadingPatternLed library using ISR for Timer0 */


#include "FadingPatternLed.h"

//buttons pin definition
#define BTN_BLUE 0
#define BTN_GREEN 13
#define BTN_RED 12

// LED pin definition
#define BLUE_LED  11
#define GREEN_LED 10
#define RED_LED   9


// Idle State paramters: duration and brightness
#define FADE_IN_TIME_IDLE  4000
#define LED_ON_TIME_IDLE   200
#define FADE_OUT_TIME_IDLE 3000
#define LED_OFF_TIME_IDLE  500
#define IDLE_BRIGHT 40

//// global variables
unsigned long currTime;
FadingPatternLed led_b(BLUE_LED, FADE_IN_TIME_IDLE, LED_ON_TIME_IDLE, FADE_OUT_TIME_IDLE, LED_OFF_TIME_IDLE, IDLE_BRIGHT);
FadingPatternLed led_g(GREEN_LED,FADE_IN_TIME_IDLE, LED_ON_TIME_IDLE, FADE_OUT_TIME_IDLE, LED_OFF_TIME_IDLE, IDLE_BRIGHT);
FadingPatternLed led_r(RED_LED,  FADE_IN_TIME_IDLE, LED_ON_TIME_IDLE, FADE_OUT_TIME_IDLE, LED_OFF_TIME_IDLE, IDLE_BRIGHT);

void setup()
{  
  //pinMode(BTN_BLUE, INPUT_PULLUP);  // not needed since analog
  pinMode(BTN_GREEN, INPUT_PULLUP);
  pinMode(BTN_RED, INPUT_PULLUP);

// START code using Interrupt, timer0 already cyclying at 1 ms rate, comparing in the middle
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
// END
  // init randomnly the seed for random, so every sketch start it will most liekly different; read an unused analog pin
  randomSeed(analogRead(0)); 
}

// START code for using ISR Timer0
SIGNAL(TIMER0_COMPA_vect)
{
  currTime = millis();

  //first update blink pattern params based on buttons pressed
  if (currTime%SAMPLING_TIME == 0)
  {// every SAMPLING_TIME msec (defined in library) ...

    // check user input to excite or not
    int lightAmbient = analogRead(BTN_BLUE);
    led_b.exciting = (lightAmbient < 50);
    led_g.exciting = (digitalRead(BTN_GREEN) == LOW);
    led_r.exciting = (digitalRead(BTN_RED) == LOW);

    // call API to update dynamically the led pattern parameters (it will use exiting state )
    led_b.updatePattern();
    led_g.updatePattern();
    led_r.updatePattern();
  }

  // update state machine and led brightness
  led_r.UpdateDisplay(currTime);
  led_b.UpdateDisplay(currTime);
  led_g.UpdateDisplay(currTime);
}
// END


void loop()
{
  // nothing to do here, every led pattern processing is done in ISR for Timer0
}
