/* led -fader example for multiask 1: just class with update polled - modified to ISR*/

/* Mantravola exp: led effects :
  TODO:
  - move all button/led pin into class
  - on press try to move up brightness to MAX and keep it till released
  - on release re-start blinking slowing it down as now */

//buttons pin definition
// since no butotn use value read by analog 0 that is brightness
#define BTN_BLUE 0
#define BTN_GREEN 13
#define BTN_RED 12

// LED pin definition
#define BLUE_LED  11
#define GREEN_LED 10
#define RED_LED   9

// LED pattern definition: states, idle/min value for fade-in/out on /off value; 
// states
#define LED_OFF 0
#define LED_ON 1
#define LED_FADE_IN 2
#define LED_FADE_OUT 3

// blink and brightness in idle state
#define IN_IDLE  4000
#define ON_IDLE   200
#define OUT_IDLE 3000
#define OFF_IDLE  500
#define  IDLE_BRIGHT 40

// blink and brightness in "excited" state
#define IN_MIN  100
#define ON_MIN  100
#define OUT_MIN 100
#define OFF_MIN 200

#define  MAX_BRIGHT 255



// led pattern adjustment value : logic is that pattern start in idle and then
// if button is pressed increase max_brightness and reduce blinking interval.
// if not pressed move back to idle pattern
#define INT_INTERACT  100  // interal interaction with button is checked and applied

// use timing to adjut values
// milli-seconds needed to ramp up/down from idle (bright/blink) to "exicited" one
#define UP_TIME 2000
#define DOWN_TIME 15000

int bright_up_step = ceil((MAX_BRIGHT-IDLE_BRIGHT)*INT_INTERACT/UP_TIME);
int bright_dw_step = ceil((MAX_BRIGHT-IDLE_BRIGHT)*INT_INTERACT/DOWN_TIME);

unsigned long blink_up_on_step  = max(1,((long)(ON_IDLE - ON_MIN))*INT_INTERACT/UP_TIME);
unsigned long blink_up_off_step = max(1,((long)(OFF_IDLE-OFF_MIN))*INT_INTERACT/UP_TIME);
unsigned long blink_up_in_step  = max(1,((long)(IN_IDLE - IN_MIN))*INT_INTERACT/UP_TIME);
unsigned long blink_up_out_step = max(1,((long)(OUT_IDLE-OUT_MIN))*INT_INTERACT/UP_TIME);

unsigned long blink_dw_on_step  = max(1,((long)(ON_IDLE - ON_MIN))*INT_INTERACT/DOWN_TIME);
unsigned long blink_dw_off_step = max(1,((long)(OFF_IDLE-OFF_MIN))*INT_INTERACT/DOWN_TIME);
unsigned long blink_dw_in_step  = max(1,((long)(IN_IDLE - IN_MIN))*INT_INTERACT/DOWN_TIME);
unsigned long blink_dw_out_step = max(1,((long)(OUT_IDLE-OUT_MIN))*INT_INTERACT/DOWN_TIME);

bool quickrampOption = true;


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

  //button pressed or not
  bool btn_pressed;
  
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
  void updatePattern()
  {
    if (btn_pressed)
    {
      //increment max bright to blink (with saturation)
      maxBright = (((long)maxBright + bright_up_step) > MAX_BRIGHT ? MAX_BRIGHT : maxBright + bright_up_step);
      
      // decrease duration of each blinking state to speed up flashing; total blnk peiod step is propotionally shared between each phase
      OnTime = OnTime - blink_up_on_step;
      OnTime = max(OnTime, ON_MIN);
      OffTime = OffTime - blink_up_off_step;
      OffTime = max(OffTime, OFF_MIN);
      fadeInTime = fadeInTime - blink_up_in_step;
      fadeInTime = max(fadeInTime, IN_MIN);
      fadeOutTime = fadeOutTime - blink_up_out_step;
      fadeOutTime = max(fadeOutTime, OUT_MIN);
    }
    else
    {
      //decrement max bright, not less then IDLE_BRIGHT
      maxBright = ((maxBright - bright_dw_step) > IDLE_BRIGHT ? maxBright - bright_dw_step : IDLE_BRIGHT);
      
      // increase duration of each blinking state to move back to idle flashing
      OnTime = OnTime + blink_dw_on_step;
      OnTime = min(OnTime, ON_IDLE);
      OffTime = OffTime + blink_dw_off_step;
      OffTime = min(OffTime, OFF_IDLE);
      fadeInTime = fadeInTime + blink_dw_in_step;
      fadeInTime = min(fadeInTime, IN_IDLE);
      fadeOutTime = fadeOutTime + blink_dw_out_step;
      fadeOutTime = min(fadeOutTime, OUT_IDLE);
    }
  }
  
  //  void Update()  - modify routine when using ISR
  void Update (unsigned long currTime)
  {

    if ((btn_pressed)&&(quickrampOption))
    {// move to excited state - till released
      analogWrite(ledPin,maxBright);
      ledState=LED_ON;
      prevTime=currTime;
      OnTime = ON_MIN;
      OffTime = OFF_MIN;
      fadeInTime = IN_MIN;
      fadeOutTime = OUT_MIN;
    }
    else 
    {
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
  Serial.println(bright_up_step);
  Serial.println(bright_dw_step);
  Serial.println(blink_dw_on_step);
  Serial.println(blink_dw_off_step);
  Serial.println(blink_dw_in_step);
  Serial.println(blink_dw_out_step);
  Serial.println(blink_up_on_step);
  Serial.println(blink_up_off_step);
  Serial.println(blink_up_in_step);
  Serial.println(blink_up_out_step);

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
    led_b.btn_pressed = (lightAmbient < 50);
    led_g.btn_pressed = (digitalRead(BTN_GREEN) == LOW);
    led_r.btn_pressed = (digitalRead(BTN_RED) == LOW);
    
    led_b.updatePattern();
    led_g.updatePattern();
    led_r.updatePattern();
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

 // Serial.println(analogRead(BTN_BLUE));
}
