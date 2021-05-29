#ifndef CONFIG_H
#define CONFIG_H

// Enable debugging
#define ENABLE_DEBUGGING 1

// Some behaviors only make sense on Teensy
#define IS_TEENSY 1

// Output pedal channel
#define MIDI_CHANNEL 4

// Velocity of note when pressed
#define MIDI_VELOCITY 64

// Pin for LED (the built-in LED is #13)
#define LED_PIN 13

/*
 * A good discussion on debouncing can be found at:
 * https://my.eng.utah.edu/~cs5780/debouncing.pdf
 * 
 * According to that, most switches tend to settle down within 10 msec.
 * Sound waves have a latency of about 1 msec per foot away.
 * Wikipedia says that latency is most noticeable when listening to your
 * own processed voice through headphones--a race between the processing
 * latency and the your own voice you hear in your own head--and in this
 * case, one study found that greater than 15 msec was noticeable.
 * So for this keyboard, I'm thinking that if we sample multiple times
 * within a 10 msec interval and have a stable reading, we will be safe.
 * Would be nice to make this interval adjustable up and down for good
 * or bad switches.
 */
#define DEBOUNCE_INTERVAL_MILLIS 10

#define LENGTH_PIN_NOTES 32

#define MAX_PINS_ARDUINO 42

#define LENGTH_POT_NUMBERS 4

const unsigned char pot_numbers[LENGTH_POT_NUMBERS][2] = {
  { 14, 0 }, // First pot
  { 15, 1 },
  { 16, 2 },
  { 17, 3 }
};

// Array of all the notes we will have.
// Each will have its own pin and will generate the given midi number
const unsigned char pin_notes[LENGTH_PIN_NOTES][2] = {
  // First 10 pins, down the upper left side of the board
  {  2, 36 }, // C2
  {  3, 37 }, // C#2
  {  4, 38 }, // D2
  {  5, 39 }, // D#2
  {  6, 40 }, // E2
  {  7, 41 }, // F2
  {  8, 42 }, // F#2
  {  9, 43 }, // G2
  {  10, 44 }, // G#2
  {  11, 45 }, // A2
  // Second row of 5 pins, up the upper right side of the board
  {  19, 46 }, // A#2
  {  20, 47 }, // B2
  {  21, 48 }, // C3
  {  22, 49 }, // C#3
  {  23, 50 }, // D3
  // Third row of 9 pins, down the lower left side of the board
  {  24, 51 }, // D#3
  {  25, 52 }, // E3
  {  26, 53 }, // F3
  {  27, 54 }, // F#3
  {  28, 55 }, // G3
  {  29, 56 }, // G#3
  {  30, 57 }, // A3
  {  31, 58 }, // A#3
  {  32, 59 }, // B3
  // Fourth row of 8 pins, up the lower right side of the board
  {  33, 60 }, // C4
  {  34, 61 }, // C#4
  {  35, 62 }, // D4
  {  36, 63 }, // D#4
  {  37, 64 }, // E4
  {  38, 65 }, // F4
  {  39, 66 }, // F#4
  {  40, 67 }, // G4
};

#endif
