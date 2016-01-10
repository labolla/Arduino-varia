// test ISR 

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  
  OCR0A = 0x88; 
  TIMSK0 |= _BV(OCIE0A);
}

SIGNAL(TIMER0_COMPA_vect)
{
  static int cnt = 0;
  unsigned long currTime = millis();
  cnt++;
  if (!(cnt%1000))  Serial.println (currTime);
}

void loop()
{
  ;
}
