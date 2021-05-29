# Health/State indicator OR Sequence Generator

  This is a header file which contains "s_seq" class and  can be used for generating sequences (either repeating endless sequences(normal) or repeating sequences with lifetime(can be used for multiple blinks)), discreate sequences with data&dummy slots for status/ health monitoring of embedded systems.

## Getting Started

### Prerequisites
  This header file is designed to work with arduino IDE. But it may also work with other IDEs. CPP language is used.

### Installing
  The folder "s_seq" can be copied to <User directory>/Documents/Arduino/library/.
The library will be automatically detected by Arduino IDE (after restarting).

  To use this class,
  Add the header file - #include "s_seq.h" 
  Creating object - s_seq a(parameters)

  Before creating object, below shown details should be identified (for sequence).
1) pin no - The sequence should be generated in which pin
2) number of elements - This will decide state_per_array and array_count
3) state_duration - actual duration of each state in milli seconds.

  Before creating object, below shown details should be identified (for health/status indication)
1) pin no - The sequence should be generated in which pin
2) number of elements - This will decide state_per_array and array_count
3) state_duration - actual duration of each state in milli seconds.
4) repeat_count - how many time the sequence should repeat immediately( before restarting)
5) dummy_states - how many dummy states are required in between sequence restarting

## Running the tests

Try the code shown below to ensure the class is working.
```#include "s_seq.h"

s_seq led(13,16,1,500);

void setup() {
  led.put_data(1,0x8800);//high in 1st and 5th state
}

void loop() {
/*     It is adviced to put user codes here;
*/
    led.play();
}
```

  This example will make a sequence of 16 states which will repeat one more time immediately and then add 16 dummy states.

  Since added data is 0x8800 (0b1000100000000000), led will blink twice with a delay of 300ms (3 states*state_duration). Then it will stay off for 3300ms (3.3sec, 11states * sate_duration). repeat_count is set as 1. So this sequence will repeat once more. Then it will be off for 1000ms (dummy_states,10 states*state_duration).
After this, the whole sequence will repeat endless.

## Coding styles
  Please note: play() should be called as fast as possible (the more frequent it is called, that much precision will be available for the sequence's state duration). If the actual code is taking large execution time, play() can be put in between (n number of time as required). play() is taking current time (time this function is called) and changing output state as required (compared to last state change time).

## Deployment

  Sample codes for some situations are mentioned in examples folder.

## License

  This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments
  To every who helped/motivated me.

