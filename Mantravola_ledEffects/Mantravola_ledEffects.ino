/* Mantravola exp: led effects :
 use FadingPatternLed library using ISR for Timer0 */


#include "FadingPatternLed.h"

//buttons pin definition
#define BTN_BLUE A0
#define BTN_GREEN 13
#define BTN_RED 12

// LED pin definition
#define BLUE_LED  11
#define GREEN_LED 10
#define RED_LED   9


// Idle State paramters: duration and brightness
#define FADE_IN_TIME_IDLE  4000
#define LED_ON_TIME_IDLE   1000
#define FADE_OUT_TIME_IDLE 3000
#define LED_OFF_TIME_IDLE  2000
#define IDLE_BRIGHT 225  // 255 is turned off

//// global variables
FadingPatternLed led_b(BLUE_LED, FADE_IN_TIME_IDLE, LED_ON_TIME_IDLE, FADE_OUT_TIME_IDLE, LED_OFF_TIME_IDLE, IDLE_BRIGHT);
FadingPatternLed led_g(GREEN_LED,FADE_IN_TIME_IDLE, LED_ON_TIME_IDLE, FADE_OUT_TIME_IDLE, LED_OFF_TIME_IDLE, IDLE_BRIGHT);
FadingPatternLed led_r(RED_LED,  FADE_IN_TIME_IDLE, LED_ON_TIME_IDLE, FADE_OUT_TIME_IDLE, LED_OFF_TIME_IDLE, IDLE_BRIGHT);


// flag indicating led pattern is started
bool red_enable = false;
bool green_enable = false;
bool blue_enable = false;

void setup()
{  
  //pinMode(BTN_BLUE, INPUT_PULLUP);  // not needed since analog
  pinMode(BTN_GREEN, INPUT_PULLUP);
  pinMode(BTN_RED, INPUT_PULLUP);

// START code using Interrupt, timer0 already cyclying at 1 ms rate, comparing in the middle
  OCR0A = 0xAF;
  //TIMSK0 |= _BV(OCIE0A);
  OCR0A = 0x88; 
  TIMSK0 |= _BV(OCIE0A);
// END
  analogWrite(BLUE_LED,255);
  analogWrite(GREEN_LED,255);
  analogWrite(RED_LED,255);
  Serial.begin(9600);
  //while (!Serial);
  Serial.println("Setup Done");
}

// START code for using ISR Timer0
SIGNAL(TIMER0_COMPA_vect)
{
//  static int cnt = 0;
  unsigned long currTime = millis();
//  cnt++;
  //if (!(cnt%1000))  Serial.println (currTime);

  //first update blink pattern params based on buttons pressed
  if (currTime%SAMPLING_TIME == 0)
  {// every SAMPLING_TIME msec (defined in library) ...

    // check user input to excite or not
    int lightAmbient = analogRead(BTN_BLUE);
    led_b.exciting = (lightAmbient > 400);
    led_g.exciting = (digitalRead(BTN_GREEN) == LOW);
    led_r.exciting = (digitalRead(BTN_RED) == LOW);

    // set enable flag to true, never set to false so pattern is enabled on first excitig action
    if (led_r.exciting)  
      red_enable = true;
    if (led_g.exciting)  
      green_enable = true;
    if (led_b.exciting)  
      blue_enable = true;

    // call API to update dynamically the led pattern parameters (it will use exiting state)
    // call only when led pattern is enabled by first exciting action
    if (blue_enable) 
      led_b.updatePattern();
    if (green_enable) 
      led_g.updatePattern();
    if (red_enable) 
      led_r.updatePattern();
  }

  // update state machine and led brightness
  // call only when led pattern is enabled by first exciting action
  if (red_enable)
    led_r.UpdateDisplay(currTime);
  if (blue_enable)
    led_b.UpdateDisplay(currTime);
  if (green_enable)
    led_g.UpdateDisplay(currTime);
}
// END


void loop()
{
  // nothing to do here, every led pattern processing is done in ISR for Timer0
}
