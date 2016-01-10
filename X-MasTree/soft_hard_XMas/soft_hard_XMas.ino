#include "pitches.h"

#define SONG_SIZE   55

int scale[] = { NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4,
                NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5};

int melody[] = {NOTE_G4,NOTE_G4, NOTE_A4, NOTE_G4, NOTE_E4, 0, 
                0, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_E4, 0,
                NOTE_D5, NOTE_D5, NOTE_B4, NOTE_B4,
                NOTE_C5, NOTE_C5, NOTE_G4, 0,
                NOTE_A4, NOTE_A4, NOTE_C5, NOTE_B4, NOTE_A4,
                NOTE_G4, NOTE_A4, NOTE_G4, NOTE_E4,0,
                NOTE_A4, NOTE_A4, NOTE_C5, NOTE_B4, NOTE_A4,
                NOTE_G4, NOTE_A4, NOTE_G4, NOTE_E4,0,
                NOTE_D5,NOTE_D5,NOTE_F5, NOTE_D5, NOTE_B4,
                NOTE_C5, NOTE_E5, 0,
                NOTE_C5, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_F4, NOTE_D4,
                NOTE_C4};

double duration [] = {1, 1.5, 0.5, 1, 2.5, 0.5,
                   1, 1.5, 0.5, 1, 2, 1,
                   2, 1, 2, 1,
                   2, 1, 2.5, 0.5,
                   2, 1, 1.5, 0.5, 1,
                   1.5, 0.5, 1, 2.5, 0.5,
                   2, 1, 1.5, 0.5, 1,
                   1.5, 0.5, 1, 2.5, 0.5,
                   2, 1, 1.5, 0.5, 1,
                   3, 2.5, 0.5,
                   1, 1, 1, 1, 1, 1,
                   3};

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

int val_delta[6] = {-10, -5, 0, +2, +4, +6 };

#define ACTIVE_DELTA  20
#define SAMPLE_PRESS 330

void display_led(int value)
{
  static int red, blue, green;
  int delta_red, delta_blue, delta_green;
  
  delta_red = value%6;
  delta_blue = ((value-delta_red)/6)%6;
  delta_green = ((value-delta_red-delta_blue*6)/36)%6;

  if ((red + val_delta[delta_red])> 255)
  {
    red = 255;
  }
  else if ((red + val_delta[delta_red])< 0)
  {
    red = 0;
  }
  else
  {
    red = red + val_delta[delta_red];
  }

  if ((green + val_delta[delta_green])> 255)
  {
    green = 255;
  }
  else if ((green + val_delta[delta_green])< 0)
  {
    green = 0;
  }
  else
  {
    green = green + val_delta[delta_green];
  }

  if ((blue + val_delta[delta_blue])> 255)
  {
    blue = 255;
  }
  else if ((blue + val_delta[delta_blue])< 0)
  {
    blue = 0;
  }
  else
  {
    blue = blue + val_delta[delta_blue];
  }

  /*Serial.print(value);
  Serial.print('\t');
  Serial.print(red);
  Serial.print('\t');
  Serial.print(blue);
  Serial.print('\t');
  Serial.println(green);*/

/*  analogWrite(RED_PIN, 255-red_level[red]);
  analogWrite(GREEN_PIN, 255-green_level[green]);
  analogWrite(BLUE_PIN, 255-blue_level[blue]);
*/
  analogWrite(RED_PIN, 255-red);
  analogWrite(GREEN_PIN, 255-green);
  analogWrite(BLUE_PIN, 255-blue);

  
}

void play_led(int duration, int pitch)
{
  int steps = duration / 50;
  static int red = 0;
  static int green = 0;
  static int blue = 0;
  int redTarget, greenTarget, blueTarget;
  int greenStep, blueStep, redStep;

  switch (pitch)
  {
  case NOTE_C4:
  case NOTE_C5:
  redTarget = 255;
  blueTarget = 0;
  greenTarget = 0;
  break;
  
  case NOTE_D4:
  case NOTE_D5:
  redTarget = 110;
  blueTarget = 150;
  greenTarget = 0;

  break;
  
  case NOTE_E4:
  case NOTE_E5:
  redTarget = 0;
  blueTarget = 255;
  greenTarget = 0;

  break;
  
  case NOTE_F4:
  case NOTE_F5:
  redTarget = 0;
  blueTarget = 110;
  greenTarget = 150;

  break;
  
  case NOTE_G4:
  case NOTE_G5:
  redTarget = 0;
  blueTarget = 0;
  greenTarget = 255;

  break;
  
  case NOTE_A4:
  case NOTE_A5:
  redTarget = 110;
  blueTarget = 0;
  greenTarget = 150;

  break;
  
  case NOTE_B4:
  case NOTE_B5:
  redTarget = 150;
  blueTarget = 110;
  greenTarget = 150;

  break;
  
  default:
  redTarget = 0;
  blueTarget = 0;
  greenTarget = 0;

  break;
  }

  redStep= (redTarget - red)/ steps;
  greenStep= (greenTarget - green)/ steps;
  blueStep= (blueTarget - blue)/ steps;
 

  for (int i = 0; i < steps; i++)
  {
    red = red + redStep;
    blue = blue + blueStep;
    green = green + greenStep;
    if (red > 255)
       red = 255;
    else if (red < 0)
       red = 0;
    if (blue > 255)
       blue = 255;
    else if (blue < 0)
       blue = 0;
    if (green > 255)
       green = 255;
    else if (green < 0)
       green = 0;
    analogWrite(RED_PIN, 255-red);
    analogWrite(GREEN_PIN, 255-green);
    analogWrite(BLUE_PIN, 255-blue);

    delay(50);
  }

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A2, INPUT_PULLUP);
  pinMode(SPK_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
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
  static int toPlay = 0;
  static unsigned int duration_cnt = 0;
  static int last_pressed_delta = 0;
  static bool starting = false;

  if (starting)
  {
    for (int thisNote = 0; thisNote < SONG_SIZE ; thisNote++)
    {
      //display_led(map(melody[thisNote]-NOTE_C3,0, NOTE_C5-NOTE_C3, 0, 215));
      int noteDuration =  600*duration[thisNote];
    
      if (melody[thisNote])
         tone(SPK_PIN, melody[thisNote]);
      else
         noTone(SPK_PIN);

      play_led(noteDuration, melody[thisNote]);
      
      noTone(SPK_PIN);
      int pauseBetweenNotes =100;
      delay(pauseBetweenNotes);
   }
   starting = false;
   Serial.println("Started");
  }

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

  Serial.print(soft_dick);
  Serial.print('\t');
  Serial.print(avr);
  Serial.print('\t');
  Serial.print(pressed_delta);
  Serial.print('\t');
  Serial.print(minVal);
  Serial.print('\t');
  Serial.println(maxVal);


  if (pressed_delta > ACTIVE_DELTA)
  {
    if (pressed_delta > ACTIVE_DELTA)
    {   
      inactive_cnt = 0;

      int noteIdx = map(pressed_delta,0, maxVal-minVal, 0, 13);
      Serial.print(noteIdx);
      Serial.print('\t');
      Serial.println(maxVal);
      tone(SPK_PIN, scale[noteIdx]);
      play_led(SAMPLE_PRESS, scale[noteIdx]);
      noTone(SPK_PIN);
    }
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
    play_led(SAMPLE_PRESS, 0);
  }

  delay(20);
}
