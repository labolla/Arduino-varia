/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
//int led = 13;
int led = 7;
int f_led=11;

int loopCnt = 2;
int numLoop = 10;
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {

// blink pattern example
/*  
  for (int i=0; i<numLoop;i++)
  {
    for (int j=0; j<loopCnt*(i+1); j++)
    {
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000/(i+1));               // wait for a second
      digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
      delay(1000/(i+1));  // wait for a second
    }
  }
  */
// fading pattern example
  for (int i = 0; i < 255; i ++)
  {
    analogWrite(f_led, i);
    delay(20+10*(128/i));
  }
  delay(1000);
  for (int i = 0; i < 255; i ++)
  {
    analogWrite(f_led, 255-i);
    delay(20);
  }
  delay(1000);
  
  for (int i=0; i<numLoop;i++)
  {
    for (int j=0; j<loopCnt*(i+1); j++)
    {
      digitalWrite(f_led, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000/(i+1));               // wait for a second
      digitalWrite(f_led, LOW);    // turn the LED off by making the voltage LOW
      delay(1000/(i+1));  // wait for a second
    }
  }
}
