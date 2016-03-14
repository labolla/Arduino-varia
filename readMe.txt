This git project collects many experimental test not strickly to be considered a final project.
List starts from basic ones to control Led and Speaker to ones performing basic audio&video effets.


####### Controlling LED and sounds

  ### my2LedBtnColorControl
  Very basic sketch to turn on and off a led based on button.

  ### myAnalogPotInput
  Use an analog Potentiomenter to control 3 leds.
  
  ### myControlledBlink
  Show how to control a led via serial command; a fading or
  number of blinking with fixed pattern can be configured.:x


  ### myAnalogPotInputSoundScale
  Play a scale (either chromatic or diatonic) based on environment light.
  Sketch use a light sensor and speaker and convert the level of light into
  an index for either chormatic or diatonic scale.

  ### myAnalogPotInputSound
  Same as myAnalogPotInputSoundScale but not map into a scale.

  ### potContlled_playDisplay
  Play a scale based on a potentionmeter; scale is built on top of scale.h and it can configured (currently at compile)
  RGB is displayed based on NewtonColorCirclePlay class associating a color to a note.
  console let first see the color, try different ones to tune (values have to be configured back in NewtonColorCirclePlay class and
  play with potentiometer)
  TODO: try out latest scale.h; and let configure the scale via console

####### Sound and Soft Pressure Button Contolling

  ### X-MasTree
  Use "soft pressure button" as input to play music based on sensed pression.
  It first start to play X-Mas song.
  TODO: use NewtonColorCirclePlay class


####### Dynamic Fading LED playing processed in ISR

  ### myISR_exp
  Simple example that show the TIMER0 ISR usage.
  It just display current time once a while. Useful to understand ISR vs loop processing execution.

  ### myLedFaderMulitTask_1
  Play and RGB pattern based on a random initiated value.
  All processing is done in ISR for timer.
  No user input to modify the played pattern

  ### Mantravola_ledEffects
  This is based on myLedFaderMulitTask_1 but get input from 2 button and a light sensor to change pattern.

