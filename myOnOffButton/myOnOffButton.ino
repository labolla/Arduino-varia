/*
  Button
  Turns on/off an LED using two buttons.
 
  This example code is in the public domain.
 */
 
 int ledPin = 5;
 int btnOn = 9;
 int btnOff = 8;
 int state = 0;

int delays[10] = {1000, 750, 500, 250, 100, 75, 50, 25, 10};
 
 void setup()
 {
   pinMode(ledPin, OUTPUT);
   pinMode(btnOn, INPUT_PULLUP);
   pinMode(btnOff, INPUT_PULLUP);
   
   while(!Serial) ;
   Serial.begin(9600);
   Serial.println("Press Upper btn to turn ON he Led and the Lower one to Turn Led OFF.");
 }
 
 void loop()
 {
   if (state == 0 && digitalRead(btnOn) == LOW)
   {
     state = 1;
     digitalWrite(ledPin, HIGH);
     Serial.println("Press Upper button, turn Led ON.");
//     fade();
   }
   if (state == 1 && digitalRead(btnOff) == LOW)
   {
     state = 0;
     digitalWrite(ledPin, LOW);
     Serial.println("Press Lower button, turn Led OFF.");
//      blink();
   }
 }

void fade()
{
  for (int i = 0; i < 255; i ++)
  {
    analogWrite(ledPin, i);
    delay(20+10*(128/i));
  }
  delay(1000);
  for (int i = 0; i < 255; i ++)
  {
    analogWrite(ledPin, 255-i);
    delay(20);
  }
  delay(1000);
}

void blink()
{
  for (int i=0; i<5;i++)
  {
    for (int j=0; j<5; j++)
    {
      digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(delays[i]);               // wait for a second
      digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
      delay(delays[i]);  // wait for a second
    }
    delay(2000);
  }
}
