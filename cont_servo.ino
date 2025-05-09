/* PWM Motor Control Sketch */


/* Uno Version
#define PWM_PIN 3
#define DIAL_CTL_PIN A0
#define DEBUG_SERIAL
#endif
*/

/* Tiny Version */
#define PWM_PIN 0
#define DIAL_CTL_PIN A1

int hi_time_us;

void setup()
{
  #ifdef DEBUG_SERIAL
  Serial.begin(9600);
  #endif

  pinMode(PWM_PIN, OUTPUT);

}


void loop()
{
  int dial_val;
  
  // Hacky, bit banged busy wait version
  dial_val = analogRead(DIAL_CTL_PIN);
  hi_time_us = 1000+dial_val;

  // if we're in the middle of the range, I'm just gonna write it low so that
  // we avoid servo wobble
  if ((dial_val < 600) && (dial_val > 400))
  {
    digitalWrite(PWM_PIN, LOW);
  }
  else
  {
    // do one pwm pulse
    digitalWrite(PWM_PIN, HIGH);
    delayMicroseconds(hi_time_us);
    digitalWrite(PWM_PIN, LOW);
    delayMicroseconds(20000-hi_time_us);
  }
}
