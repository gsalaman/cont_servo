/* PWM Motor Control Sketch */


/* Uno Version only...uses serial*/
#define PWM_PIN 3
#define DIAL_CTL_PIN A0


int hi_time_us=1500;
int current_steps = 0;
int max_num_steps = 0;

typedef enum
{
  MODE_CONTINUOUS,
  MODE_STEPS
}  mode_type;

mode_type current_mode = MODE_CONTINUOUS;

void set_us_delay(int number)
{
  hi_time_us = number;
  Serial.print("Pulse set to ");
  Serial.println(hi_time_us);
}

void set_num_steps(int steps)
{
  current_steps = 0;
  max_num_steps = steps;

  Serial.print("Setting number of steps to ");
  Serial.println(steps);
}

void print_menu(void)
{
  Serial.println("COMMANDS:");
  Serial.println("  c to set continous mode");
  Serial.println("      number then sets pulse width in us");
  Serial.println("  s to set step mode");
  Serial.println("      number then sets number of steps to take.");
  Serial.println("      servo will stop after that number of steps");
  Serial.println("================================================");
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
            if (current_mode == MODE_CONTINUOUS)
            {
              set_us_delay(number);
            }
            else if (current_mode == MODE_STEPS)
            {
              set_num_steps(number);
            }
            else
            {
              Serial.print("Unknown mode in building number: ");
              Serial.println(current_mode);
            }
            
            number = 0;
            building_number=false;
          }
        break;

        case 'c':
          Serial.println("Setting mode to continuous");
          current_mode = MODE_CONTINUOUS;
        break;

        case 's':
          Serial.println("Setting mode to steps");
          current_mode = MODE_STEPS;
        break;
        
        default:
          Serial.print("Unknown input: ");
          Serial.println(c);
          print_menu();
      }
    }
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(PWM_PIN, OUTPUT);

  print_menu();

}


void loop()
{

    process_serial();

    switch (current_mode)
    {
      case MODE_CONTINUOUS:
        // do one pwm pulse
        digitalWrite(PWM_PIN, HIGH);
        delayMicroseconds(hi_time_us);
        digitalWrite(PWM_PIN, LOW);
        delayMicroseconds(20000-hi_time_us);
      break;

      case MODE_STEPS:
        //do a pulse only if we haven't done enough steps yet
        if (current_steps < max_num_steps)
        {
          // do one pwm pulse
          digitalWrite(PWM_PIN, HIGH);
          delayMicroseconds(hi_time_us);
          digitalWrite(PWM_PIN, LOW);
          delayMicroseconds(20000-hi_time_us);
          current_steps++;
        }
        break;

      default:
        Serial.print("Unknown mode in loop(): ");
        Serial.println(current_mode);
    }
    
}
