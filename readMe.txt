This got project collect many experimental test not stickly to be considered a final project.
List starts from basic ones to control Led and Speaker to perform basic audio&video effets.


####### Controlling LED and sounds
  
  TODO: use serial console to select analog/diatonic/chromatic sound playing merging
        myAnalogPotInputSoundScale and myAnalogPotInputSound.
        Re-use LED class from next section to play a LED based on pitch so that we have:
        Sensd light -> mapped into a note -> display LED accordinly.

  ### my2LedBtnColorControl
  Very basic sketch to turn on and off a led based on button.

  ### myAnalogPotInput
  Use an analgo Potentiomenter to control 3 leds.
  
  ### myControlledBlink
  Show how to control a led via serial command; a fading or
  number of blinking with fixed pattern can be configured.

  ### myAnalogPotInputSoundScale
  Play a scale (either chromatic or diatonic) based on environment light.
  Sketch use a light sensor and speaker and convert levle of light into
  an index for either chormatic or diatonic scale.

  ### myAnalogPotInputSound
  Same as myAnalogPotInputSoundScale but not map into a scale.

####### Sound and Soft Pressure Button Contolling

  TODO: define a LED class that display RGB value based on a music note.
        change the mapping based on https://technochicblog.wordpress.com/2012/10/22/the-real-relationship-between-sound-and-color/

  ### X-MasTree
  Use "soft pressure button" as input to play music based on sensed pression.
  It first start to play X-Mas song.


####### Dynamic Fading LED playing processed in ISR

  TODO: defined a library for a Fading LED that currently is used only by Mantravola_ledEffects.
        status: verify only compilation; need to verify functionality and re-use in myLedFaderMulitTask_1

  ### myISR_exp
  Simple example that show the TIMER0 ISR usage.
  It just display current time once a while. Useful to understand ISR vs loop processing execution.

  ### myLedFaderMulitTask_1
  Play and RGB pattern based on a random initiated value.
  All processing is done in ISR for timer.
  No user input to modify the played pattern

  ### Mantravola_ledEffects
  This is based on myLedFaderMulitTask_1 but get input from 2 button and a light sensor to change pattern.






