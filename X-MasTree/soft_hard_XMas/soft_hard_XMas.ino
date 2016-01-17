#include "pitches.h"

#define DIATONIC_SCALE

#if defined (DIATONIC_SCALE)
int scale[] = { NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4,
                NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5};
#endif

//#define XMAS_MELODY
#if defined (XMAS_MELODY)
#define MELODY
/* Xmas melody section:
   define melody size, pitches (0 means silence) and duration */
#define SONG_SIZE   55
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
#endif

// GPIOs defines for LED, speaker and "analog soft button"
#define RED_PIN  5
#define GREEN_PIN 6
#define BLUE_PIN 9
#define SPK_PIN  10
#define SOFT_PIN A2

// MIN_MAX minimum delta "press sensor" is considered calibrated
#define PEAK_2_PEAK 20

// minumum analog pressed value to consider the soft button been pressed
#define ACTIVE_DELTA  20

// duration of a pitch in ms
#define SAMPLE_DURATION_MS 330
// rfresh period in ms for RB led when playing a new note
#define RGB_REFRESH_PERIOD_MS      50


// routine to play the RGB led based on pitch; 
// LED color is not played immediately but it is reached from current status to desired one in 50 ms long step
void play_led(int duration, int pitch)
{
  // static variable to save current RGB led value (0 is off and 255 is on; it will be inversed on displaying based on used LED)
  static int red = 0;
  static int green = 0;
  static int blue = 0;
  // desired RGB led value mapping the pitch being played
  int redTarget, greenTarget, blueTarget;
  // RGB is adjusted every
  int steps = duration / RGB_REFRESH_PERIOD_MS;
  int greenStep, blueStep, redStep;

//TODO: extend to cover all chromatics scales; my change color for note on different octaves (C4 and C5)
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

    delay(RGB_REFRESH_PERIOD_MS);
  }

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SOFT_PIN, INPUT_PULLUP);
  pinMode(SPK_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  // static value to tace min, max, average of last 8 samples, absolute max and min and consecutive inactive press
  static int minVal = 2000;
  static int maxVal = 0;
  static int avr = 0;
  static int absMaxVal = 0;
  static int absMinVal = 2000;
  static int inactive_cnt=0;
  //"press sensor" analog value
  int soft_pressure = 0;
  // delta between min "press sensor" value and current one; it measure current press action
  int pressed_delta;
  // prev pressed_delta to keep track if pressure sensor is not "blocking"
  static int prev_pressed_delta = 0;
  static int is_blocking_cnt = 0;
  static bool press_sensor_calibrated = false;
  static bool play_melody = true;
  // to swing alternatevely do a 2/3 and 1/3,.... i.e. alternatevely divide by 2 sample_duration
  static int swing_state = 0;
  int sample_duration;

#if defined(MELODY)
  if (play_melody)
  {
    Serial.print("Playing Melody ... ");
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
   play_melody = false;
   Serial.println("... done. You can press to play !!");
  }
#endif

  soft_pressure = analogRead(SOFT_PIN);

  absMaxVal = max(soft_pressure, absMaxVal);
  absMinVal = min(soft_pressure, absMinVal);
  avr = (avr * 7 + soft_pressure) / 8;

  if (((avr - absMinVal) > PEAK_2_PEAK/2) && ((absMaxVal- avr) > PEAK_2_PEAK/2))
  {
      maxVal = max(avr, maxVal);
      minVal = min(avr, minVal);
      press_sensor_calibrated = true;      
  }

  pressed_delta = avr - minVal;

  Serial.print(soft_pressure);
  Serial.print('\t');
  Serial.print(avr);
  Serial.print('\t');
  Serial.print(pressed_delta);
  Serial.print('\t');
  Serial.print(minVal);
  Serial.print('\t');
  Serial.println(maxVal);
  
  if (swing_state)
  {
    sample_duration = SAMPLE_DURATION_MS / 2;
    swing_state = 0;
  }
  else
  {
    sample_duration = SAMPLE_DURATION_MS;
    swing_state = 1;
  }

  if (pressed_delta > ACTIVE_DELTA)
  {  
      inactive_cnt = 0;

      int noteIdx = map(pressed_delta,0, maxVal-minVal, 0, 13);
      Serial.print(noteIdx);
      Serial.print('\t');
      Serial.println(maxVal);
      tone(SPK_PIN, scale[noteIdx]);
      play_led(sample_duration, scale[noteIdx]);
      noTone(SPK_PIN);

      if ((pressed_delta == prev_pressed_delta) &&  (pressed_delta > ACTIVE_DELTA*2))
      {
        is_blocking_cnt++;
        if (is_blocking_cnt == 4 )
        {
          minVal = avr;
        }
      }
  }
  else
  {
    if (press_sensor_calibrated)
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
    play_led(sample_duration, 0);
  }

  prev_pressed_delta = pressed_delta;

  delay(20);
}
