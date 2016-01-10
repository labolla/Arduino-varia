/*
 *  Analog Input with Pot
*/

int PotPin = 0;
int oldPot = 0;
int minPot = 2000;
int maxPot = 1;
int cnt = 0;

int led_1 = 9;
int led_2 = 10;
int led_3 = 11;

void setup()
{
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  Serial.begin(9600);
  while (!Serial) ;
  //Serial.println("Ciao Irene ora il papa gira il POT!!!");
  
}

void loop()
{
  int potPos = analogRead(PotPin);
  if (oldPot != potPos)
  {
    cnt++;
    Pot2Led(potPos);
    //Serial.print("Irene il pot e' nella posizione ");
    //Serial.print(potPos);
    //Serial.println(" . Gira ancora");
    maxPot = max(potPos, maxPot);
    minPot = min(potPos, minPot);
    if (cnt == 500)
    {
      Serial.print("MaxLight value = ");
      Serial.println(maxPot);
      Serial.print("MinLight value = ");
      Serial.println(minPot);
      cnt = 0;
    }
    oldPot = potPos;
  }
  delay(10);
}

void Pot2Led(int pos)
{
  int led_1_val = 0;
  int led_2_val = 0;
  int led_3_val = 0;
  
  if (pos < 120)
  {
    Serial.print("MIN error tuning ");
    Serial.println (pos);
  }
  if (pos > 1020)
  {
    Serial.print("MAX error tuning ");
    Serial.println (pos);
  }
  
  
  // experimented that 120 is for max light and 1020 for min light
  // so substract 120 and covert from 1020-120=990 range to 768 for the 3 led 256 value range
  int conv_pos = (pos-120);
  if (conv_pos < 256)
  {
    led_1_val = conv_pos;
  }
  else if (conv_pos < 512)
  {
    led_1_val = 255;
    led_2_val = conv_pos - 256; 
  }
  else 
  {
    led_1_val = 255;
    led_2_val = 255;
    if (conv_pos >= 768)
       led_3_val = 255;
    else
       led_3_val = conv_pos - 512; 
  }
  
  Serial.println(pos);
  Serial.println(conv_pos);
  Serial.println(led_1_val);
  Serial.println(led_2_val);
  Serial.println(led_3_val);
  Serial.println("");
  
  analogWrite(led_1, led_1_val);
  analogWrite(led_2, led_2_val);
  analogWrite(led_3, led_3_val);
}

