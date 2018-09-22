//=============================================================================================================//
//                                                                                                             //
//  ## Health Indicator / Sequence Generator - Library ##                                                      //
//  This is an Arduino compatible library for showing                                                          //
//  current state of Arduino. This can be used for displaying                                                  //
//  sequences also                                                                                             //
//                                                                                                             //
//  Filename : s_seq.h                                                                                         //
//  Description : Part of s_seq library - definition.                                                          //
//  Library version : 0.2                                                                                      //
//  Author : Manu M                                                                                            //
//  Source : https://github.com/muralysunam/Health-State-Indicator-OR-Sequence-Generator/tree/master/s_seq     //
//  Initial release : +05:30 09:42 PM, 22-09-2018, Saturday                                                    //
//  License : MIT                                                                                              //
//                                                                                                             //
//  File last modified : +05:30 09:42 PM, 22-09-2018, Saturday                                                 //
//                                                                                                             //
//=============================================================================================================//


#ifndef s_seq_h

#define s_seq_h
#define temp_array_cnt 30
#define dummy_bit 0
#include "Arduino.h"
 
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
