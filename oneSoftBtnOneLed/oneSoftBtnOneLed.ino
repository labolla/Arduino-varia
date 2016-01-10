#include <CapacitiveSensor.h>

CapacitiveSensor HR_4_2 = CapacitiveSensor(2,4);
CapacitiveSensor LR_5_2 = CapacitiveSensor(2,5);
CapacitiveSensor Ce_6_2 = CapacitiveSensor(2,6);
CapacitiveSensor LL_7_2 = CapacitiveSensor(2,7);
CapacitiveSensor HL_8_2 = CapacitiveSensor(2,8);


int red_pin = 9;
int red_bright = 255;
int green_pin = 10;
int green_bright = 255;
int blue_pin = 11;
int blue_bright = 255;


void setup() {
  Serial.begin(9600);
  pinMode(red_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
}

void loop() {
  long start = millis();
  long HR_value =  HR_4_2.capacitiveSensor(30);
  long LR_value =  LR_5_2.capacitiveSensor(30);
  long Ce_value =  Ce_6_2.capacitiveSensor(30);
  long LL_value =  LL_7_2.capacitiveSensor(30);
  long HL_value =  HL_8_2.capacitiveSensor(30);

  if (HR_value > 400)
  {// touched, increase brigthness
      if (red_bright != 0)  red_bright=red_bright-5;  
  }
  else
  {// not touched, decrease brightness
      if (red_bright != 255)  red_bright=red_bright+5;  
  }

  if (LR_value > 400)
  {// touched, increase brigthness
      //if (blue_bright != 0)  blue_bright=blue_bright-5;
      //if (green_bright != 0)  green_bright=green_bright-5;    
  }
  else
  {// not touched, decrease brightness
      //if (blue_bright != 255)  blue_bright=blue_bright+5;
      //if (green_bright != 255)  green_bright=green_bright+5;  
  }

  if (LL_value > 400)
  {// touched, increase brigthness
      //if (blue_bright != 0)  blue_bright=blue_bright-5;
      //if (red_bright != 0)  red_bright=red_bright-5;    
  }
  else
  {// not touched, decrease brightness
      //if (blue_bright != 255)  blue_bright=blue_bright+5;
      //if (red_bright != 255)  red_bright=red_bright+5;    
  }

  if (HL_value > 400)
  {// touched, increase brigthness
      if (blue_bright != 0)  blue_bright=blue_bright-5;  
  }
  else
  {// not touched, decrease brightness
      if (blue_bright != 255)  blue_bright=blue_bright+5;  
  }

  if (Ce_value > 400)
  {// touched, increase brigthness
      if (green_bright != 0)  green_bright=green_bright-5;  
  }
  else
  {// not touched, decrease brightness
      if (green_bright != 255)  green_bright=green_bright+5;  
  }

 // Serial.print(millis() - start);        // check on performance in milliseconds
 // Serial.print("\t");                    // tab character for debug windown spacing
 // Serial.print(HR_value);                  // print sensor output 1
  analogWrite(green_pin, green_bright);
  analogWrite(red_pin, red_bright);
  analogWrite(blue_pin, blue_bright);
  
  delay(10);                             // arbitrary delay to limit data to serial port 

}
