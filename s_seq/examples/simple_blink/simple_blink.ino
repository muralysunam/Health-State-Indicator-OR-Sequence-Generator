/*
 * This is an example file for s_seq.h 
 *
 * This will blink inbuilt LED for an event(if Serial data is available).
 * s_seq library is availble https://github.com/muralysunam/Health-State-Indicator-OR-Sequence-Generator/tree/master/s_seq 
 * 
 * This example code is written by Manu M on +05:30 23/09/2018 16:30
 * 
 *This example code is in the public domain.
*/

#include "s_seq.h"

#define led_pin 13
#define led_state_per_array 2
#define led_array_count 1
#define led_repeat_count 0
#define led_state_duration 500 // in milli seconds
#define led_dummy_state_count 0 

#define blink_repetition 2 //no of blink required for one event. Dont use 0


void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  s_seq led(led_pin,led_state_per_array,led_array_count,led_state_duration);
  while(1)
  {
    if(Serial.available())
    {
      led.put_data(1,0x01,blink_repetition);
      led.reset();
      Serial.print("Received:   ");
      Serial.write(Serial.read());
      Serial.println();
    }
    led.play();
  }
  
    
}
