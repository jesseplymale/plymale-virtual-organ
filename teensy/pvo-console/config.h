#ifndef CONFIG_H
#define CONFIG_H

// Enable debugging
#define ENABLE_DEBUGGING 1
#define DEBUG_LED_PIN 13
#define HALF_SECOND_MILLIS 500

// Some behaviors only make sense on Teensy
#define IS_TEENSY 1

// Debouncing for the pistons
#define DEBOUNCE_INTERVAL_MILLIS 10

// On each virtual MIDI cable, the outgoing messages are sent
// on this channel (regardless of what channel they were sent on
// when they were read from the hardware serial port)
// Set this to 0 if you want to use whatever port the keyboard sent
// it on.
#define USB_MIDI_CHANNEL 1

#define LENGTH_POT_NUMBERS 4

const unsigned char pot_numbers[LENGTH_POT_NUMBERS][2] = {
  { 14, 0 }, // First pot
  { 15, 1 },
  { 16, 2 },
  { 17, 3 }
};

#endif
