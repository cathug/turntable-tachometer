# A more stable Arduino-based Turntable Tachometer
---

This respository contains all required files for the Turntable Tachometer.


---
## Why reinvent the wheel?
Existing Arduino tachometer code that is flowing around the internet give 
erratic readings even when the IR sensor is properly debounced.  They are not 
useful for speed sensitive devices like a turntable.

The code in this repository introduces a few improvements:
1. Intantaneous speed readings are saved to a circular array.
After the IR sensor collects the tenth reading, the average reading flag toggles
and an average speed parameter is displayed on the LCD screen.
2. Display response speed and memory usage are improved as the next reading will
overwrite the earliest reading in the circular array.
3. The timer interrupt routine handles the calculations periodically.  The loop
only performs LCD display tasks.



---
## Required hardware and electrical components
* Arduino ATMega328 based controller - I am using Arduino Duemilanove
* Nokia 5110 LCD display
* Custom IR module - *see schematics for more details*

The code is written for **ATMega328-based Arduinos** in mind.  Slight
modifications are required before compiling the source code for other Arduinos.

---
## Custom IR module
The Omron EE-SF5 IR sensor is used but can be subsituted with any IR LED and 
Phototransistor pairings.
