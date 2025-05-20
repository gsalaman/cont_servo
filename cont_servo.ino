/* PWM Motor Control Sketch */


/* Uno Version only...uses serial*/
#define PWM_PIN 3
#define DIAL_CTL_PIN A0


int hi_time_us=1000;

void set_us_delay(int number)
{
  hi_time_us = number;
  Serial.print("Pulse set to ");
  Serial.println(hi_time_us);
}

void process_serial( void )
{
  char c;
  static bool building_number=false;
  static uint32_t  number = 0;
  
  /* commands:
   *  f = fast motion
   *  d = switch direction
   *  x = stop
   *  s = slow motion
   *  any number sets the microsecond delay
   */
  if (Serial.available())
  {
    c = Serial.read();

    if (c >= '0' && c <= '9')
    {
      building_number = true;
      
      /* continue building our number input */
      number = number * 10;
      number = number + (c - '0');   
    }

    else
    {
        
      switch (c)
      {
        case '\n':
        case '\r':
        case '*':
          if (building_number)
          {
            set_us_delay(number);
        
            number = 0;
            building_number=false;
          }
        break;

        default:
          Serial.print("Unknown input: ");
          Serial.println(c);
      }
    }
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(PWM_PIN, OUTPUT);

}


void loop()
{

    process_serial();
    
    // do one pwm pulse
    digitalWrite(PWM_PIN, HIGH);
    delayMicroseconds(hi_time_us);
    digitalWrite(PWM_PIN, LOW);
    delayMicroseconds(20000-hi_time_us);

}
