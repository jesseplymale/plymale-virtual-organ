# Using the Teensy 4.1 Microcontroller

I use two Teensy 4.1 microcontrollers in my project.

## `pvo-console`

Features include:

* 8 midi serial inputs
* scans a 12x9 matrix of piston buttons
* Merges all the inputs into a single MIDI serial output
* Puts each midi serial input into its own virtual MIDI cable over USB
* Provides a virtual MIDI cable over USB to send the events for the piston button presses

## `pvo-pedalboard`

Features include:

* Uses 32 input pins to connect to 32 notes of an organ pedalboard
* Provides 4 analog inputs to connect Yamaha FC7 pedals to
* Outputs the MIDI