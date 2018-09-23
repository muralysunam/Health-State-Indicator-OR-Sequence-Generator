/*
 * This is an example file for s_seq.h 
 * 
 * Buzzer should be connected to pin number 4.
 * If buzzer is not available, you can use arduino's LED to see output
 * For using LED, change pin number from 4 to 13 in line 18
 * This will beep/blink every minutes. Beep count depends on current minute. It will reset after 10 minutes.
 * You can know the time passed once the program started(it repeates after 10 counts)
 * s_seq library is availble https://github.com/muralysunam/Health-State-Indicator-OR-Sequence-Generator/tree/master/s_seq 
 * 
 * This example code is written by Manu M on +05:30 23/09/2018 15:00
 * 
 *This example code is in the public domain.
*/

#include "s_seq.h"

#define buzzer_pin 4
#define buzzer_state_per_array 32 // you can use upto 16 beeps
#define buzzer_array_count 1
#define buzzer_repeat_count 0
#define buzzer_state_duration 300 // in milli seconds
#define buzzer_dummy_state_count 0

#define event_time 0.25 //in minutes 
#define blink_repetition 1 //no of times to repeat,0 for no ending


void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  //object creations
  s_seq buzzer(buzzer_pin,buzzer_state_per_array,buzzer_array_count,buzzer_state_duration);
  byte counter;
  unsigned long int start_time = millis(), temp_data = 0;
  while(1)
  {
    if((millis()-start_time) >= (float) event_time*60*1000)
    {
      if(counter < 10)
      {
        counter++;
      }
      else
      {
        temp_data = 0;
        counter = 1;
      }
      bitSet(temp_data,32-(counter*2)-1);//in order to set odd bits(MSB first)
      start_time = millis();
      buzzer.put_data(1,temp_data,blink_repetition);
      buzzer.reset();
      Serial.print(millis());
      Serial.print("     ");
      Serial.print(start_time);
      Serial.print("     ");
      Serial.println(counter);
    }
    buzzer.play();
  }
    
}
