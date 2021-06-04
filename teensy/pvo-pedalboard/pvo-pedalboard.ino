/**
 * Reads 32 input pins as note switches. Outputs result as MIDI
 * through serial interface and USB interface. Flashes built-in
 * LED whenever a NOTE ON is sent, for use in hooking up pedals.
 * Provides inputs for expression pedals as well.
 * 
 * TODO:
 *  * Currently pin 40, 43, and 52 all have initial note_off messages
 *    for some reason. Perhaps the pullup resistor takes some time to kick in?
 */

// For inspiration, see:
// https://github.com/PaulStoffregen/Teensyduino_Examples/blob/master/USB_MIDI/Many_Button_Knobs/Many_Button_Knobs.ino

// include the ResponsiveAnalogRead library for analog smoothing
#include <ResponsiveAnalogRead.h>
// include the v2 Bounce library for 'de-bouncing' switches
#include <Bounce2.h>
// bring in our configuration
#include "config.h"
// Used to hook up the hardware serial MIDI
// usbMIDI.h library is added automatically when code is compiled as a MIDI device
#include <MIDI.h>

// Initialize the hardware MIDI port
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

// This is the array of Bounce buttons. To be able to map more
// quickly from the pin index
Bounce *debouncers[LENGTH_PIN_NOTES];

// Array of ResponsiveAnalogRead for reading potentiometers smoothly
ResponsiveAnalogRead *pot_readers[LENGTH_POT_NUMBERS];

// a lagged copy to tell when MIDI changes are required for potentiometer
// when lag and new are not the same then update MIDI CC value
char old_pot_values[LENGTH_POT_NUMBERS];

/**
 * Runs once on startup
 */
void setup() {
  // Set up the serial object, if we are doing debugging and printing
  // out to the Arduino Serial Monitor
  if (ENABLE_DEBUGGING) {
    // Set up the serial for debugging
    if (!IS_TEENSY) {
      // This is for Arduino compatibility and Teensy doesn't need it
      Serial.begin(38400);
    }
    while (!Serial) {
      // Wait for the Arduino Serial Monitor has opened
      // This should not be run in a standalone application without the serial monitor
    }
  }

  // This means the midi port can receive on all channels.
  // Doesn't make a difference since at this point we don't use any incoming data
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();

  // Set up the LED, and have it not turned on at first
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Set up the switches
  for (unsigned char i=0; i<LENGTH_PIN_NOTES; i++) {
    const unsigned char *pin_note = pin_notes[i];
    unsigned char pin = pin_note[0];
    unsigned char note = pin_note[1];

    if (ENABLE_DEBUGGING) {
      Serial.print("Setting up pin=");
      Serial.print(pin);
      Serial.print(" with note=");
      Serial.println(note);
    }

    // Instantiate the debouncer
    Bounce *debouncer = new Bounce();
    debouncer->attach(pin, INPUT_PULLUP);
    debouncer->interval(DEBOUNCE_INTERVAL_MILLIS);
    debouncers[i] = debouncer;
  }

  // Set up potentiometers
  for (unsigned char i=0; i<LENGTH_POT_NUMBERS; i++) {
    const unsigned char *pot_number = pot_numbers[i];
    unsigned char pin = pot_number[0];
    unsigned char cc = pot_number[1];
    if (ENABLE_POT_DEBUGGING) {
      Serial.print("Setting up pot on pin=");
      Serial.print(pin);
      Serial.print(" with cc=");
      Serial.println(cc);
    }
    // The pot_reader we are about to set.
    // The ResponsiveAnalogRead will set the pin as an INPUT
    ResponsiveAnalogRead *pot_reader = new ResponsiveAnalogRead(pin, true);
    pot_readers[i] = pot_reader;
  }

  // Put this here to try to get away from the momentary
  // signal we got on a few pins on startup
  delay(100);

  // Set the original state for each of the buttons
  for (int i=0; i<LENGTH_PIN_NOTES; i++) {
    // Get the note that corresponds to the debouncer
    unsigned char note = pin_notes[i][1];

    // Get the debouncer and update its state
    Bounce *debouncer = debouncers[i];
    debouncer->update();

    // Do a note off for all the notes at the beginning, so that
    // a restart of the device can silence any hanging notes
    note_changed(note, 0);
  }
}

/**
 * Trigger the behavior needed when the given note number is
 * turned on or off
 */
void note_changed(unsigned char note_number, int is_note_on) {
  if (ENABLE_DEBUGGING) {
    if (is_note_on) {
      Serial.print("Note ON note=");
    } else {
      Serial.print("Note OFF note=");
    }
    Serial.println(note_number);
  }

  // Set the LED
  int led_pin_value = is_note_on ? HIGH : LOW;
  digitalWrite(LED_PIN, led_pin_value);

  // Send the MIDI Note
  if (is_note_on) {
    // Send the MIDI note through the hardware serial port
    MIDI.sendNoteOn(note_number, MIDI_VELOCITY, MIDI_CHANNEL);
    // Send through the USB MIDI as well
    usbMIDI.sendNoteOn(note_number, MIDI_VELOCITY, MIDI_CHANNEL);
  } else {
    // Same as NoteOn, but for NoteOff
    MIDI.sendNoteOff(note_number, 0, MIDI_CHANNEL);
    usbMIDI.sendNoteOff(note_number, 0, MIDI_CHANNEL);
  }
}

/**
 * Return back a value from 0 to 127, derived from the value retrieved
 * from the potentiometer.
 */
char translate_pot_cc_value(int raw_analog_value) {
  // If the value is under this minimum, we set it to 0. That way
  // if for some reason the pot won't go all the way to 0, we can still
  // go to zero with our CC
  if (raw_analog_value < MIN_POT_RESISTANCE) {
    raw_analog_value = MIN_POT_RESISTANCE;
  }

  // Make sure that if the pot goes over the MAX_POT_RESISTANCE,
  // we chop it down to the max here
  if (raw_analog_value > MAX_POT_RESISTANCE) {
    raw_analog_value = MAX_POT_RESISTANCE;
  }

  int min_max_range = MAX_POT_RESISTANCE - MIN_POT_RESISTANCE;
  int pot_value_in_range = raw_analog_value - MIN_POT_RESISTANCE;

  // Adding a small number (the divisor minus one) to the dividend will
  // make it round up when doing the scaling division.
  int rounding_addition = min_max_range - 1;
  int scaled_value = ((pot_value_in_range * MAX_CC_VALUE) + rounding_addition) / min_max_range;

  // In case I made a mistake above, make sure the result is within the max
  if (scaled_value > MAX_CC_VALUE) {
    scaled_value = MAX_CC_VALUE;
  }

  return (char)scaled_value;
}

/**
 * Listen to the potentiometers and send midi messages as needed
 */
void handle_pots() {
  // Set up potentiometers
  for (unsigned char i=0; i<LENGTH_POT_NUMBERS; i++) {
    const unsigned char *pot_number = pot_numbers[i];
    unsigned char cc = pot_number[1];

    // The pot_reader we are about to use
    ResponsiveAnalogRead *pot_reader = pot_readers[i];
    // update the value
    pot_reader->update();

    if (pot_reader->hasChanged()) {
      char new_value = translate_pot_cc_value(pot_reader->getValue());
      char old_value = old_pot_values[i];
      if (new_value != old_value) {
        // Send the midi message
        MIDI.sendControlChange(cc, new_value, MIDI_CHANNEL);
        usbMIDI.sendControlChange(cc, new_value, MIDI_CHANNEL);

        // Update the old value array
        old_pot_values[i] = new_value;

        if (ENABLE_POT_DEBUGGING) {
          Serial.print("cc=");
          Serial.print(cc, DEC);
          Serial.print(" value=");
          Serial.println(new_value, DEC);
        }
      }
    }
  }
}

/**
 * Listen to the pins and see what notes have been pressed, if any.
 */
void handle_notes() {
  // Check the debouncers
  for (int i=0; i<LENGTH_PIN_NOTES; i++) {
    // Get the note that corresponds to the debouncer
    unsigned char note = pin_notes[i][1];

    // Get the debouncer and update its state
    Bounce *debouncer = debouncers[i];
    debouncer->update();

    if (debouncer->fell()) {
      // Since we are assuming an active low (with internal pullup),
      // a falling edge to ground will indicate that the button has
      // just been pressed
      note_changed(note, 1);
      
    } else if (debouncer->rose()) {
      // This means the note has been released
      note_changed(note, 0);
    }
  } 

  while (MIDI.read()) {
    // This is to drain the input buffer just in case something is trying to
    // send MIDI information here.
  }
  while (usbMIDI.read()) {
    // Need to drain the USB MIDI input buffer as well
  }
}

/**
 * Runs continuously while the program is running
 */
void loop() {
  handle_notes();
  handle_pots();
}
