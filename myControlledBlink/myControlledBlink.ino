/*
  Controlled Blink
  Contorl led by serial..
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
//int led = 13;
int led = 7;
int f_led=11;

int numPattern = 9;
int delays[10] = {1000, 750, 500, 250, 100, 75, 50, 25, 10};
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  Serial.begin(9600);
//  while (!Serial);
  Serial.println("Type 'f' to fade the led or type number of blinks for led pattern (10 pattern will be played)");
}

// the loop routine runs over and over again forever:
void loop() {

  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch == 'f')
    {
      Serial.print("Starting fading led ...");
      fadingLed();
      Serial.println("... completed!!");
    }
    else if (ch >= '0' && ch <= '9')
    {
      int numBlinks = ch - '0';
      Serial.print("Starting blinking led for number of blinks ");
      Serial.print(numBlinks);
      Serial.print("...");
      ledBlink(numBlinks);
      Serial.println("... completed!!");      
    }
  }  
}

void fadingLed()
{
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
}

void ledBlink(int numBlinks)
{
  for (int i=0; i<numPattern;i++)
  {
    for (int j=0; j<numBlinks; j++)
    {
      digitalWrite(f_led, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(delays[i]);               // wait for a second
      digitalWrite(f_led, LOW);    // turn the LED off by making the voltage LOW
      delay(delays[i]);  // wait for a second
    }
    delay(2000);
  }
}
