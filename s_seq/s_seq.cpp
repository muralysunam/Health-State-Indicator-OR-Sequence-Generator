#include "Arduino.h"
#include "s_seq.h"


s_seq::s_seq(byte t_pin, byte t_state_per_array, byte t_array_count, byte t_repeat_count, unsigned long int t_state_duration, unsigned long int t_dummy_state_count) : pin(t_pin), state_per_array(t_state_per_array), array_count(t_array_count), repeat_count(t_repeat_count), state_duration(t_state_duration), active_state_count(t_state_per_array*t_array_count), total_state_count(active_state_count+t_dummy_state_count)  {}
//above - generic constructor - used for health monitoring
s_seq::s_seq(byte t_pin, byte t_state_per_array, byte t_array_count, unsigned long int t_state_duration) : pin(t_pin), state_per_array(t_state_per_array), array_count(t_array_count), repeat_count(0x00), state_duration(t_state_duration), active_state_count(t_state_per_array*t_array_count), total_state_count(0x00)  {}
//above - constructor - used for sequence repetition alone - no repetition and dummy states

//The above shown lines are used for initializing const variables.


void s_seq::init() // This for is for pin mode defining and resetting data to zero
{
	pinMode(pin,OUTPUT);
	reset_data();
}
  
void s_seq::play()	// This is the main function
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
							clear_data(i-1);
						else
							lifetime[i-1]--;
					}
				}
			}
			else	//If dummy states are required, we will add dummy state and increment counter
			{
				//Serial.println(current_state_counter);
				//show_data();
				digitalWrite(pin,_dummy_bit);
				current_state_counter++;
			}
			last_state_start_time = millis(); // updating current state's time for next state's referance
		}
	}
}

void s_seq::reset() // resetting counter to zero means sequence will start again
{
	current_state_counter = 0;
}

void s_seq::put_data(byte t_seq_no, unsigned long int data)// one method to enter data. lifetime will be infinite 
{
    seq[t_seq_no-1] = data;
    lifetime[t_seq_no-1] = 0;
}

void s_seq::put_data(byte t_seq_no, unsigned long int temp_data, byte temp_lifetime)//overloaded; this method can be used if you need to specify lifetime
{
    seq[t_seq_no-1] = temp_data;
    lifetime[t_seq_no-1] = temp_lifetime;
}

void s_seq::show_data()// This method is used for Serial out-ing current data of all sequences
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

void s_seq::clear_data(byte t_seq_no)//clear data of selected sequence
{
    put_data(t_seq_no, 0x00,0);
}

void s_seq::reset_data()//clear data of all sequence
{
    for(int i = 1; i <= array_count; i++)
		put_data(i, 0x00,0);
}


