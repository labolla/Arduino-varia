/* led -fader example for multiask 1: just class with update polled - modified to ISR*/

#define BLUE_LED  11
#define GREEN_LED 10
#define RED_LED   9

#define LED_OFF 0
#define LED_ON 1
#define LED_FADE_IN 2
#define LED_FADE_OUT 3

#define  IN_IDLE  400
#define  ON_IDLE  20
#define  OUT_IDLE  300
#define  OFF_IDLE  50
  
#define  IDLE_STEP  10


class LedFader
{
  public: 
  // Class member variables
  // pattern definition
  int ledPin;
  long OnTime;
  long OffTime;
  long fadeInTime;
  long fadeOutTime;
    
  // current state
  int ledState;  // FADE_IN, ON, FADE_OUT, OFF
  signed long prevTime;
  
  // constructor - create LedFader
  //public:
  LedFader(int pin, long fadeIn, long on, long fadeOut, long off)
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
    
  }
  
//  void Update()  - modify routine when using ISR
void Update (unsigned long currTime)
  {
    // get current millisecond value - not needed when using ISR, get it once and pass as paramter to update
 //   unsigned long currTime = millis();  
    
    // check state machine, update status and prevTime, compute fade value if needed and update pin
    if ((ledState==LED_OFF)&&(currTime-prevTime >= OffTime))
    {//OFF->FADE_IN
      ledState=LED_FADE_IN;
      prevTime=currTime;
      int fadeValue = (currTime-prevTime)*255/fadeInTime;
      analogWrite(ledPin,fadeValue);
    }
    else if (ledState==LED_FADE_IN)
    {
      if (currTime-prevTime >= fadeInTime)
      {//FADE_IN->ON
        ledState=LED_ON;
        prevTime=currTime;
        analogWrite(ledPin,255);
      }
      else
      {//just update fade value
        int fadeValue = (currTime-prevTime)*255/fadeInTime;
        analogWrite(ledPin,fadeValue);
      }
    }
    else if ((ledState==LED_ON)&&(currTime-prevTime >= OnTime))
    {//ON->FAD_OUT
      ledState=LED_FADE_OUT;
      prevTime=currTime;
      int fadeValue = 255 - (currTime-prevTime)*255/fadeOutTime;
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
        int fadeValue = 255 - (currTime-prevTime)*255/fadeOutTime;
        analogWrite(ledPin,fadeValue);
      }
    }
  }
};

LedFader led_b(BLUE_LED, IN_IDLE*IDLE_STEP, ON_IDLE*IDLE_STEP, OUT_IDLE*IDLE_STEP, OFF_IDLE*IDLE_STEP);
LedFader led_g(GREEN_LED,IN_IDLE*IDLE_STEP, ON_IDLE*IDLE_STEP, OUT_IDLE*IDLE_STEP, OFF_IDLE*IDLE_STEP);
LedFader led_r(RED_LED,  IN_IDLE*IDLE_STEP, ON_IDLE*IDLE_STEP, OUT_IDLE*IDLE_STEP, OFF_IDLE*IDLE_STEP);

void setup()
{
  Serial.begin(9600);
  while (!Serial) ;
//  Serial.println("Trace fading value");
  Serial.println("Led \t  initValue"); 
  Serial.print(led_r.ledPin); Serial.print("\t"); Serial.println(led_r.prevTime);
  Serial.print(led_b.ledPin); Serial.print("\t"); Serial.println(led_b.prevTime);
  Serial.print(led_g.ledPin); Serial.print("\t"); Serial.println(led_g.prevTime);
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
  unsigned long currTime = millis();
  
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
}
