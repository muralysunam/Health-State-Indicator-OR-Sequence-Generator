//=============================================================================================================//
//                                                                                                             //
//  ## Health Indicator / Sequence Generator - Library ##                                                      //
//  This is an Arduino compatible library for showing                                                          //
//  current state of Arduino. This can be used for displaying                                                  //
//  sequences also                                                                                             //
//                                                                                                             //
//  Filename : s_seq.cpp                                                                                       //
//  Description : Part of s_seq library.                                                                       //
//  Library version : 0.3                                                                                      //
//  Author : Manu M                                                                                            //
//  Source : https://github.com/muralysunam/Health-State-Indicator-OR-Sequence-Generator/tree/master/s_seq     //
//  Initial release : +05:30 09:42 PM, 22-09-2018, Saturday                                                    //
//  License : MIT                                                                                              //
//                                                                                                             //
//  File last modified : +05:30 04:13 PM, 23-09-2018, Saturday                                                 //
//                                                                                                             //
//=============================================================================================================//

#include "Arduino.h"
#include "s_seq.h"

//constructor - used for health monitoring
s_seq::s_seq(byte t_pin, byte t_state_per_array, byte t_array_count, byte t_repeat_count, unsigned long int t_state_duration, unsigned long int t_dummy_state_count) : pin(t_pin), state_per_array(t_state_per_array), array_count(t_array_count), repeat_count(t_repeat_count), state_duration(t_state_duration), active_state_count(t_state_per_array*t_array_count), total_state_count(active_state_count+t_dummy_state_count)  
{
	pinMode(pin,OUTPUT);// This for is for pin mode defining and resetting data to zero
	reset_data();
}

//constructor - used for sequence repetition alone
s_seq::s_seq(byte t_pin, byte t_state_per_array, byte t_array_count, unsigned long int t_state_duration) : pin(t_pin), state_per_array(t_state_per_array), array_count(t_array_count), repeat_count(0x00), state_duration(t_state_duration), active_state_count(t_state_per_array*t_array_count), total_state_count(0x00)  
{
	pinMode(pin,OUTPUT);// This for is for pin mode defining and resetting data to zero
	reset_data();
}

//This function assess called time and writes current state to digital pin
void s_seq::play()
{
	if(current_state_counter == 0) //on the first iteration, we need to initialize start_time 
	{
		last_state_start_time = millis(); //This start time will be initialized every sequence reset
		digitalWrite(pin,bitRead(seq[0],(state_per_array - 1))); // this is the first bit needed to be executed.
		current_state_counter = 1;
	}
	if((millis()-last_state_start_time) > state_duration) //time is calculated from start_time// state_duration is controlled by this 'if'
	{
		if(current_state_counter < (active_state_count * (repeat_count+1)))//this is valid until sequence is completed(sequence with repetition)  
		{
			buffer_addr_calc = (unsigned long int) ((current_state_counter)/(state_per_array))%array_count;
			buffer_bit_calc = (state_per_array-(current_state_counter%state_per_array))-1;
			digitalWrite(pin,bitRead(seq[buffer_addr_calc],buffer_bit_calc)); //We need to find MSB of the seq and then upto LSB.
			/*Serial.print(buffer_addr_calc);  // if you uncomment these lines, you can see the seq no and bit position used
			Serial.print("     ");
			Serial.print(buffer_bit_calc);
			Serial.print("     ");
			Serial.print(bitRead(seq[buffer_addr_calc],buffer_bit_calc));
			Serial.println("     ");*/
			current_state_counter++;
		}
		else// once all elements in sequence is completed(including repetition), it will reach here
		{
			if(current_state_counter >= total_state_count) //This will check whether any dummy state is required (return false if dummy state is required)
			{
				current_state_counter = 0;	//if dummy states are not required OR if all dummy states are completed, sequence reset by reseting counter
				temp_count = array_count; 	//this is just a temperory location to store array count so that we can use while loop.
				for( int i=array_count; i > 0; i--)
				{
					//Serial.println(lifetime[temp_count-1]);
					if(lifetime[i-1]) 	//all sequence's life lime is checked here. If life time is 1, that sequence is cleared else decremented. If zero, infinite life
					{
						if(lifetime[i-1] == 1)
							clear_data(i);
						else
							lifetime[i-1]--;
					}
				}
			}
			else	//If dummy states are required, we will add dummy state and increment counter
			{
				//Serial.println(current_state_counter);
				//show_data();
				digitalWrite(pin,dummy_bit);
				current_state_counter++;
			}
		}
		last_state_start_time = millis(); // updating current state's time for next state's referance
	}
}

//This will reset the current sequence and sequence will start from first state
void s_seq::reset()
{
	current_state_counter = 0;
}

//method to enter data to sequence. Data to each slot should be entered seperatly.
void s_seq::put_data(byte t_seq_no, unsigned long int data)// one method to enter data. lifetime will be infinite 
{
    seq[t_seq_no-1] = data;
    lifetime[t_seq_no-1] = 0;
}

//Another method to enter data. Here you can define lifetime of the data too.
void s_seq::put_data(byte t_seq_no, unsigned long int temp_data, byte temp_lifetime)
{
    seq[t_seq_no-1] = temp_data;
    lifetime[t_seq_no-1] = temp_lifetime;
}

// This function is used to display current sequence data via serial port
// Serial.begin() should be initilized on main program
void s_seq::show_data()
{
	for(int i = 1 ; i <= array_count; i++)//extract array no 1 to max
	{ 
		Serial.print(i);
		Serial.print("      ");
		for(int j = state_per_array ; j > 0  ; j--) //to extract MSB to LSB
		{
			Serial.print(bitRead( seq[i-1] , (j - 1) ));
		}
		Serial.print("      ");
		Serial.println(lifetime[i-1]);
	}
}

//This function can be used to clear data of selected array/slot
void s_seq::clear_data(byte t_seq_no)
{
    put_data(t_seq_no, 0x00,0);
}

//Clear data of sequence
void s_seq::reset_data()
{
    for(int i = 1; i <= array_count; i++)
		put_data(i, 0x00,0);
}


