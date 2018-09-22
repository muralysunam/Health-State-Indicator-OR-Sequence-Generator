#ifndef s_seq_h

#define s_seq_h
#define temp_array_cnt 30
#define dummy_bit 0
#include "Arduino.h"


/*
* This is a generic class (I tried to make it as generic as possible
* Using this class, you can push a sequence(runtime variable) through a digital output pin
* Please note : This can't be used effectively if you are using delay in your program.
*/
 
 
class s_seq
{
  private:
  const byte pin,state_per_array, array_count, repeat_count;
  const unsigned long int state_duration, active_state_count, total_state_count;
  byte lifetime[temp_array_cnt], buffer_addr_calc = 0, buffer_bit_calc = 0, temp_count = 0;
  unsigned long int seq[temp_array_cnt];
  unsigned long int current_state_counter = 0,last_state_start_time = 0;
  
  public:
  s_seq(byte t_pin, byte t_state_per_array, byte t_array_count, byte t_repeat_count, unsigned long int t_state_duration, unsigned long int t_dummy_state_count);
  s_seq(byte t_pin, byte t_state_per_array, byte t_array_count, unsigned long int t_state_duration);
  void play();
  void reset();
  void put_data(byte t_seq_no, unsigned long int t_data);
  void put_data(byte t_seq_no, unsigned long int t_data, byte t_lifetime);
  void clear_data(byte t_seq_no);
  void show_data();
  void reset_data();
};

#endif
