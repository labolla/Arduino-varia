/* led -fader example for multiask 1: just class with update polled - modified to ISR*/

/* Mantravola exp: led effects :
  TODO:
  - FIX interval adjustment based on pressed or not; not working when not pressed
  - each button is changing one led:
    - when pressed increase by step the brightness and reduce, or else, velocity of on/off 
    - when not pressed for a time, move down both velocity and brightness to idle state */

//buttons pin definition
// since no butotn use value read by analog 0 that is brightness
#define BTN_BLUE 0
#define BTN_GREEN 13
#define BTN_RED 12

// LED pin definition
#define BLUE_LED  11
#define GREEN_LED 10
#define RED_LED   9

// LED pattern definition: states, idle value for fade-in/out on /off value; a multiply idle value and brigthness level
#define LED_OFF 0
#define LED_ON 1
#define LED_FADE_IN 2
#define LED_FADE_OUT 3

#define  IN_IDLE  4000
#define  ON_IDLE  200
#define  OUT_IDLE  3000
#define  OFF_IDLE  500
  

#define  MAX_BRIGHT 255
#define  IDLE_BRIGHT 40

#define IN_MIN 100
#define ON_MIN 100
#define OUT_MIN 100
#define OFF_MIN 200

// led pattern adjustment value : logic is that pattern start in idle and then
// if button is pressed increase max_brightness and reduce blinking interval.
// if not pressed move back to idle pattern
#define INT_INTERACT  100  // interal interaction with button is checked and applied
#define BRIGHT_UP   5   // step to increase if button is pressed
#define BRIGHT_DOWN 2   // step to decrease if button not pressed
#define BLINK_UP 50     // number of step tomove from _IDLE to zero value 
#define BLINK_DOWN 150   // number of step to move from _MIN to _IDLE value

class LedFader
{
  public: 
  // Class member variables
  // pattern definition
  // need to be public to be accessed by extern when printing value
  int ledPin;
  long OnTime;
  long OffTime;
  long fadeInTime;
  long fadeOutTime;
  int maxBright;
    
  // current state
  int ledState;  // FADE_IN, ON, FADE_OUT, OFF
  signed long prevTime;
  
  // constructor - create LedFader
  //public:
  LedFader(int pin, long fadeIn, long on, long fadeOut, long off, int max_bright)
  {
    ledPin = pin;
    pinMode(ledPin, OUTPUT);
    
    OnTime = on;
    OffTime = off;
    fadeInTime = fadeIn;
    fadeOutTime = fadeOut;
    
    ledState = LED_OFF;
    //prevTime = 0;
    // init randomly the curr off time
    prevTime = random((pin-RED_LED)*fadeInTime/3, (pin+1-RED_LED)*fadeInTime/3);

    maxBright = max_bright;
    
  }
  
  // updatePattern: called every interactive interval with buttonPressed info. It adjusts blinking parameters
  void updatePattern(bool pressed)
  {
    if (pressed)
    {
      //increment max bright to blink (with saturation)
      maxBright = (((long)maxBright + BRIGHT_UP) > MAX_BRIGHT ? MAX_BRIGHT : maxBright + BRIGHT_UP);
      
      // decrease duration of each blinking state to speed up flashing
      OnTime = OnTime - ON_IDLE/BLINK_UP;
      OnTime = max(OnTime, ON_MIN);
      OffTime = OffTime - OFF_IDLE/BLINK_UP;
      OffTime = max(OffTime, OFF_MIN);
      fadeInTime = fadeInTime - IN_IDLE/BLINK_UP;
      fadeInTime = max(fadeInTime, IN_MIN);
      fadeOutTime = fadeOutTime - OUT_IDLE/BLINK_UP;
      fadeOutTime = max(fadeOutTime, OUT_MIN);
    }
    else
    {
      //decrement max bright, not less then IDLE_BRIGHT
      maxBright = ((maxBright - BRIGHT_DOWN) > IDLE_BRIGHT ? maxBright - BRIGHT_DOWN : IDLE_BRIGHT);
      
      // increase duration of each blinking state to move back to idle flashing
      OnTime = OnTime + ON_IDLE/BLINK_DOWN;
      OnTime = min(OnTime, ON_IDLE);
      OffTime = OffTime + OFF_IDLE/BLINK_DOWN;
      OffTime = min(OffTime, OFF_IDLE);
      fadeInTime = fadeInTime + IN_IDLE/BLINK_DOWN;
      fadeInTime = min(fadeInTime, IN_IDLE);
      fadeOutTime = fadeOutTime + OUT_IDLE/BLINK_DOWN; 
      fadeOutTime = min(fadeOutTime, OUT_IDLE);
    }
  }
  
  //  void Update()  - modify routine when using ISR
  void Update (unsigned long currTime)
  {
    // get current millisecond value - not needed when using ISR, get it once and pass as paramter to update
 //   unsigned long currTime = millis();  
    
    // check state machine, update status and prevTime, compute fade value if needed and update pin
    if ((ledState==LED_OFF)&&((signed long) (currTime-prevTime) >= OffTime))
    {//OFF->FADE_IN
      ledState=LED_FADE_IN;
      prevTime=currTime;
    }
    else if (ledState==LED_FADE_IN)
    {
      if (currTime-prevTime >= fadeInTime)
      {//FADE_IN->ON
        ledState=LED_ON;
        prevTime=currTime;
        analogWrite(ledPin,maxBright);
      }
      else
      {//just update fade value
        int fadeValue = (currTime-prevTime)*maxBright/fadeInTime;
        // if fadeTime has changed we may have been gone above maxBright
        fadeValue = min(fadeValue, maxBright);
        analogWrite(ledPin,fadeValue);
      }
    }
    else if ((ledState==LED_ON)&&(currTime-prevTime >= OnTime))
    {//ON->FAD_OUT
      ledState=LED_FADE_OUT;
      prevTime=currTime;
      int fadeValue = maxBright;
      analogWrite(ledPin,fadeValue);
    }
    else if (ledState==LED_FADE_OUT)
    {
      if (currTime-prevTime >= fadeOutTime)
      {//FADE_OUT->OFF
        ledState=LED_OFF;
        prevTime=currTime;
        analogWrite(ledPin,0);
      }
      else
      {//just update fade value
        int fadeValue = maxBright - (currTime-prevTime)*maxBright/fadeOutTime;
        // if fadeOutTime has changed we may have gone above maxBright in negative delta.
        fadeValue = max(fadeValue, 0);
        analogWrite(ledPin,fadeValue);
      }
    }
  }
};

//// global variables
unsigned long currTime;
LedFader led_b(BLUE_LED, IN_IDLE, ON_IDLE, OUT_IDLE, OFF_IDLE, IDLE_BRIGHT);
LedFader led_g(GREEN_LED,IN_IDLE, ON_IDLE, OUT_IDLE, OFF_IDLE, IDLE_BRIGHT);
LedFader led_r(RED_LED,  IN_IDLE, ON_IDLE, OUT_IDLE, OFF_IDLE, IDLE_BRIGHT);

void setup()
{
  Serial.begin(9600);
  while (!Serial) ;
//  Serial.println("Trace fading value");
  Serial.println("Led \t  initValue"); 
//  Serial.print(led_r.ledPin); Serial.print("\t"); Serial.println(led_r.maxBright);
//  Serial.print(led_b.ledPin); Serial.print("\t"); Serial.println(led_b.maxBright);
//  Serial.print(led_g.ledPin); Serial.print("\t"); Serial.println(led_g.maxBright);

  //pinMode(BTN_BLUE, INPUT_PULLUP);  // not needed since analog
  pinMode(BTN_GREEN, INPUT_PULLUP);
  pinMode(BTN_RED, INPUT_PULLUP);

// START code using Interrupt, timer0 already cyclying at 1 ms rate, copmaring in the middle
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
// END
  // init randomnly the seed for random, so every sketch start it will most liekly different; read an unused analog pin
  randomSeed(analogRead(0)); 
}

// START code for using ISR - put here what i was in loop, getting mills once
SIGNAL(TIMER0_COMPA_vect)
{
  currTime = millis();

  //first update blink pattern params based on buttons pressed
  if (currTime%INT_INTERACT == 0)
  {// verify button pressed every INT_INTERACT msec
    int lightAmbient = analogRead(BTN_BLUE);
    
    bool bluePressed = (lightAmbient < 50);
    bool greenPressed = (digitalRead(BTN_GREEN) == LOW);
    bool redPressed = (digitalRead(BTN_RED) == LOW);
    
    led_b.updatePattern(bluePressed);
    led_g.updatePattern(greenPressed);
    led_r.updatePattern(redPressed);
  }

  // now play update led
  led_r.Update(currTime);
  led_b.Update(currTime);
  led_g.Update(currTime);
}
// END


void loop()
{
  /* not needed using ISR
  led_r.Update();
  led_b.Update();
  led_g.Update();
  */

  Serial.println(analogRead(BTN_BLUE));
}
