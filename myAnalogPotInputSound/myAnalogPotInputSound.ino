/*
 *  Analog Input with Pot
*/

int PotPin = 0;
int oldPot = 0;
// variable to tune Mix detected Light
int minPot = 2000;
int maxPot = 1;
// below one are automatically tuned
int minLight = 2000;
int maxLight = 100;

int rangeTune = 300;

int cnt = 0;

int led_1 = 9;
int led_2 = 10;
int led_3 = 6;
int spk = 7;

void setup()
{
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(spk, OUTPUT);
  analogWrite(led_1, 255);
  analogWrite(led_2, 255);
  analogWrite(led_3, 255);
  
  delay(1000);
  Serial.begin(9600);
  while (!Serial) ;
  Serial.println("Play With teremin!!!");
  
}

void loop()
{
  int potPos = analogRead(PotPin);
  //just do something if different from previous value
  if (oldPot != potPos)
  {
    // tune read light intensitive
    maxPot = max(potPos, maxPot);
    minPot = min(potPos, minPot);
    // tun range for mapping: it may be forced to not pass a certain value to avoid difficulty in have it back
    if (minPot < minLight)   minLight = minPot;
    if (maxPot > maxLight)   maxLight = maxPot;
    if (cnt == 500)
    {
//      Serial.print("MaxLight value = ");
//      Serial.println(maxPot);
//      Serial.print("MinLight value = ");
//      Serial.println(minPot);
      cnt = 0;
    }
    
    cnt++;

    if ((maxLight - minLight) > rangeTune)
    {
      //map read Light value into a 0-768 range for the 3 led intensity of 256 each
      int thisLight = map(potPos, minLight, maxLight, 0, 768);
      
      // map read Light value into a 120-1500 Hz pitch range to be played out
      int thisPitch = map(potPos, minLight, maxLight, 120, 1500);
     
      // now display light and play pitch
      Pot2Led(thisLight);
      tone(spk, thisPitch);
    } 
    else
       Serial.println(maxLight - minLight);
    
    oldPot = potPos;
  }
  
  delay(10);
}

void Pot2Led(int pos)
{
  int led_1_val = 0;
  int led_2_val = 0;
  int led_3_val = 0;
  
  if (pos < 256)
     led_1_val = pos;
  else if (pos < 512)
  {
    led_1_val = 255;
    led_2_val = pos - 255;
  }
  else
  {
    led_1_val = 255;
    led_2_val = 255;
    led_3_val = pos - 511;
  }  
//  Serial.println(pos);
//  Serial.println(led_1_val);
//  Serial.println(led_2_val);
//  Serial.println(led_3_val);
//  Serial.println("");
  
  analogWrite(led_1, led_1_val);
  analogWrite(led_2, led_2_val);
  analogWrite(led_3, led_3_val);
}

