#ifndef CONFIG_H
#define CONFIG_H

// Enable debugging
#define ENABLE_DEBUGGING 0
#define DEBUG_LED_PIN 13
#define HALF_SECOND_MILLIS 500

// Some behaviors only make sense on Teensy
#define IS_TEENSY 1

// Debouncing for the pistons
#define DEBOUNCE_INTERVAL_MILLIS 10

// Hold interval may be unnecessary in our case
#define HOLD_INTERVAL_MILLIS 1000

// On each virtual MIDI cable, the outgoing messages are sent
// on this channel (regardless of what channel they were sent on
// when they were read from the hardware serial port)
// Set this to 0 if you want to use whatever port the keyboard sent
// it on.
#define USB_MIDI_CHANNEL 1

// We send piston messages over virtual midi cable "PVO1 CONSOLE MIDI 9" (index 8)
// and we send the message over the concatenated midi port on channel 9
#define PISTON_MSG_CABLE_NUM 8
#define PISTON_MSG_CONCAT_PORT_CHANNEL 9

// We do program changes for most of the buttons, but for this one
// button (SET) we use a CC message since we need to detect when
// the button is held down. We send a value of 0 for OFF and 127 for ON.
#define SET_BUTTON_NUMBER 76
#define SET_BUTTON_CC

///// Piston (keypad)

// Rows are the input side
const byte ROWS = 9;

// Columns are the output side
const byte COLS = 12;

// Connect to the row pinouts of the keypad
// These will read the pulse sent by the colPins
 byte rowPins[ROWS] = {  2, 3, 4, 5, 6, 9, 10, 11, 12 };

// Connect to the column pinouts of the keypad
// These will be set sequentially as outputs and given a pulse high/low
byte colPins[COLS] = { 18, 19, 30, 31, 32, 33, 49, 50, 51, 52, 53, 54 };

char keys[ROWS][COLS] = {
  {   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12 },
  {  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24 },
  {  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36 },
  {  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48 },
  {  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60 },
  {  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72 },
  {  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84 },
  {  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96 },
  {  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108 }
};

#endif
