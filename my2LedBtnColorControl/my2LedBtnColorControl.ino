/*
  Button
  Turns on/off an LED using two buttons.
 
  This example code is in the public domain.
 */
 
 int RedPin = 6;
 int WhitePin = 5;
 int btnRed = 9;
 int btnWhite = 8;
 int red = 0;
 int white = 0;

 void setup()
 {
   pinMode(RedPin, OUTPUT);
   pinMode(WhitePin, OUTPUT);
   pinMode(btnRed, INPUT_PULLUP);
   pinMode(btnWhite, INPUT_PULLUP);
 }
 
 void loop()
 {
   if (digitalRead(btnRed) == LOW && digitalRead(btnWhite) == LOW)
   {
       red=0;
       white=0;
   }
   else
   {
     if (digitalRead(btnRed) == LOW)
     {
       red++;
       if (red > 255) red=0;
     }
     if (digitalRead(btnWhite) == LOW)
     {
       white++;
       if (white > 255) white=0;
     }
   }
   
   analogWrite(RedPin, red);
   analogWrite(WhitePin, white);
   
   
   delay(100);
 }
