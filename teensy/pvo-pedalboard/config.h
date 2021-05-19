#ifndef CONFIG_H
#define CONFIG_H

// Enable debugging
#define ENABLE_DEBUGGING 0

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

// Array of all the notes we will have.
// Each will have its own pin and will generate the given midi number
const unsigned char pin_notes[LENGTH_PIN_NOTES][2] = {
  // First row of pins, down the upper left side of the board
  {  2, 36 }, // C2
  {  3, 37 }, // C#2
  {  4, 38 }, // D2
  {  5, 39 }, // D#2
  {  6, 40 }, // E2
  {  7, 41 }, // F2
  {  8, 42 }, // F#2
  {  9, 43 }, // G2
  // Second row of pins, up the upper right side of the board
  {  16, 44 }, // G#2
  {  17, 45 }, // A2
  {  18, 46 }, // A#2
  {  19, 47 }, // B2
  {  20, 48 }, // C3
  {  21, 49 }, // C#3
  {  22, 50 }, // D3
  {  23, 51 }, // D#3
  // Third row of pins, down the lower left side of the board
  {  24, 52 }, // E3
  {  25, 53 }, // F3
  {  26, 54 }, // F#3
  {  27, 55 }, // G3
  {  28, 56 }, // G#3
  {  29, 57 }, // A3
  {  30, 58 }, // A#3
  {  31, 59 }, // B3
  // Fourth row of pins, up the lower right side of the board
  {  34, 60 }, // C4
  {  35, 61 }, // C#4
  {  36, 62 }, // D4
  {  37, 63 }, // D#4
  {  38, 64 }, // E4
  {  39, 65 }, // F4
  {  40, 66 }, // F#4
  {  41, 67 }, // G4
};

#endif
