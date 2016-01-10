
// LED defines
int RED_PIN = 5;
int GREEN_PIN = 6;
int BLUE_PIN = 9;

int SPK_PIN = 10;

const int soft_pin = A2;
//const int hard_pin = A5;
#define DELTA 10

int red_level[6] =  {0, 10, 30, 70, 140, 250};
int blue_level[6] =  {0, 10, 30, 70, 140, 250};
int green_level[6] =  {0, 10, 30, 70, 140, 250};

#define ACTIVE_DELTA  20

void display_led(int value)
{
  int red, blue, green;
  
  red = value%6;
  blue = ((value-red)/6)%6;
  green = ((value-red-blue*6)/36)%6;

  Serial.print(value);
  Serial.print('\t');
  Serial.print(red);
  Serial.print('\t');
  Serial.print(blue);
  Serial.print('\t');
  Serial.println(green);

  analogWrite(RED_PIN, 255-red_level[red]);
  analogWrite(GREEN_PIN, 255-green_level[green]);
  analogWrite(BLUE_PIN, 255-blue_level[blue]);
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A2, INPUT_PULLUP);
  pinMode(SPK_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  for (int i = 0 ; i < 6; i++)
  {
    for (int j = 0 ; i < 6; i++)
    {
      for (int k = 0 ; i < 6; i++)
      {
        analogWrite(GREEN_PIN, 255 -green_level[i]);
        analogWrite(BLUE_PIN, 255 -blue_level[j]);
        analogWrite(RED_PIN, 255 -red_level[k]);
        delay(500);
      }
    }     
  }
}

void loop() {
  int soft_dick = 0;
  static int minVal = 2000;
  static int maxVal = 0;
  static int avr = 0;
  static int absMaxVal = 0;
  static int absMinVal = 2000;
  static int inactive_cnt=0;
  int pressed_delta;
  static bool started = false;

  soft_dick = analogRead(soft_pin);

  absMaxVal = max(soft_dick, absMaxVal);
  absMinVal = min(soft_dick, absMinVal);
  avr = (avr * 7 + soft_dick) / 8;

  if (((avr - absMinVal) > DELTA) && ((absMaxVal- avr) > DELTA))
  {
      maxVal = max(avr, maxVal);
      minVal = min(avr, minVal);
      started = true;
  }

  pressed_delta = avr - minVal;

  /*Serial.print(soft_dick);
  Serial.print('\t');
  Serial.print(avr);
  Serial.print('\t');
  Serial.print(pressed_delta);
  Serial.print('\t');
  Serial.print(minVal);
  Serial.print('\t');
  Serial.println(maxVal);*/

  if (pressed_delta > ACTIVE_DELTA)
  {
    inactive_cnt = 0;
    tone(SPK_PIN, pressed_delta*5);
    //remap value to 0-6^3 range
    display_led(map(pressed_delta,0, maxVal-minVal, 0, 215));
  }
  else
  {
    if (started)
    {
      //reset min once a while
      inactive_cnt = inactive_cnt + 1;
      if (inactive_cnt == 10)
      {
        inactive_cnt = 0;
        minVal = avr;  //may be replaced with avr during inactive window
      }
    }
    noTone(SPK_PIN);
    display_led(0);
  }

  delay(100);
}
