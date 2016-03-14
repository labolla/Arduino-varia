#include <NewtonColorCirclePlay.h>
#include <pitches.h>

#include <scales.h>


#define  POT_PIN   A1

#define SPK_PIN    8
#define RED_PIN    6
#define GREEN_PIN 10
#define BLUE_PIN   9


//define only one type of scales
#define CHROMATIC      0
#define IONIC          1
#define PENTATONIC     0
#define BLUES          0

//define first note of the scale, and number of octave; NOT do not pass max note that is NOTE_DS8
#define FIRST_NOTE    (OCTAVE_3_IDX + C_OFFSET)
#define NUM_OCTAVE    2
int* scale_p =  &scale_chromatic[FIRST_NOTE];

// relative idx of the pitch (offest from scale_p)
int idx = 0;

#if CHROMATIC
#define SCALE_SIZE  CHROMATIC_SIZE
int* offset_p = chromatic_offset;
#endif
#if IONIC
#define SCALE_SIZE  DIATONIC_SIZE
int* offset_p = ionic_offset;
#endif
#if PENTATONIC
#define SCALE_SIZE  PENTATONIC_SIZE
int* offset_p = pentatonic_minor;
#endif
#if BLUES
#define SCALE_SIZE  BLUES_SIZE
int* offset_p = blues_minor;
#endif

void tuningHelp()
{
  Serial.println("RGB led tuning for tone : start Display C and move chromatically. Type:");
  Serial.println("- 'u' to move up by half tone");
  Serial.println("- 'd' to move down by half tone");
  Serial.println("- 's' to set a specific r-g-b triple values");
  Serial.println("- 'P' to start play with potentiometer");
  
}

NewtonColorCirclePlay  rgb_newton_sound(RED_PIN, GREEN_PIN, BLUE_PIN, 0, COMMON_ANODE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) ;
  //Serial.println("Play With Pot!!!");
  tuningHelp();
}

void loop() {
  // put your main code here, to run repeatedly:
  static int old_pot_value = 10000;
  int delta;
  static bool tuning = true;
  bool changed = false;
  bool read_done;

  if (tuning)
  {
    if (Serial.available())
    {
      char ch = Serial.read();
      switch (ch)
      {
        case 'u':
        changed = true;
        idx++;
        if (idx == SCALE_SIZE)
          idx = 0;
        break;

        case 'd':
        changed = true;
        if (idx == 0)
          idx = SCALE_SIZE -1;
        else
          idx--;
        break;

        case 'P':
        tuning = false;
        break;

        case 's':
        Serial.println("give me RGB as: r g b");
        read_done=false;
        while (read_done==false)
        {
          while ((Serial.available() > 1))
          {
           // look for the next valid integer in the incoming serial stream:
           int red = Serial.parseInt();
           // do it again:
           int green = Serial.parseInt();
           // do it again:
           int blue = Serial.parseInt();
           
           rgb_newton_sound.SetRGB(red, green, blue);
           
           read_done = true;
          }
        }
        break;

        default:
        tuningHelp();
        break;
      }
      if (changed)
        rgb_newton_sound.Display(scale_p[offset_p[idx]], 0);
    }  
  }
  //else
  {
    int pot_value = analogRead(POT_PIN);

    if (pot_value > old_pot_value)
    {
      delta = pot_value - old_pot_value;
    }
    else
    {
      delta = old_pot_value - pot_value;
    }

    if (delta > 15)
    {
      old_pot_value = pot_value;

      int idx_scale = map(pot_value, 0, 1024, 0, NUM_OCTAVE * SCALE_SIZE);
      Serial.print("idx_scale = ");
      Serial.println(idx_scale);
      int thisPitch = scale_p[offset_p[idx]];      

      tone(SPK_PIN, thisPitch);
      // later remap to scale
      rgb_newton_sound.Display(thisPitch, 0);
    }
    else
    {
      noTone(8);
    }
  }

  delay(100);

  
}
