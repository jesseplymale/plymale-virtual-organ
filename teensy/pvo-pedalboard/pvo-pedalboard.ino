/**
 * Reads 32 input pins as note switches. Outputs result as MIDI
 * through serial interface and USB interface. Flashes built-in
 * LED whenever a NOTE ON is sent, for use in hooking up pedals.
 * 
 * TODO Currently pin 40, 43, and 52 all have initial note_off messages
 *      for some reason. Perhaps the pullup resistor takes some time to kick in?
 */
 
#include <Metro.h>
#include <Bounce2.h>
#include "config.h"
#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

// This is how often (how many milliseconds) we will poll the switches
Metro pin_poll_interval = Metro(1);

Metro pot_poll_interval = Metro(250);

// This is the array of Bounce buttons. To be able to map more
// quickly from the pin index
Bounce debouncers[LENGTH_PIN_NOTES];

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

    // The debouncer we are about to set
    Bounce *debouncer = &debouncers[i];

    // Instantiate the debouncer
    *debouncer = Bounce();
    debouncer->attach(pin, INPUT_PULLUP);
    debouncer->interval(DEBOUNCE_INTERVAL_MILLIS);
  }

  // Set up potentiometers
  for (unsigned char i=0; i<LENGTH_POT_NUMBERS; i++) {
    const unsigned char *pot_number = pot_numbers[i];
    unsigned char pot = pot_number[0];
    unsigned char number = pot_number[1];
    if (ENABLE_POT_DEBUGGING) {
      Serial.print("Setting up pot=");
      Serial.print(pot);
      Serial.print(" with number=");
      Serial.println(number);
    }
    pinMode(pot, INPUT);
  }

  delay(100);
  
  for (int i=0; i<LENGTH_PIN_NOTES; i++) {
    // Get the note that corresponds to the debouncer
    unsigned char note = pin_notes[i][1];

    // Get the debouncer and update its state
    Bounce *debouncer = &debouncers[i];
    debouncer->update();

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
 * Listen to the potentiometers and send midi messages as needed
 */
void handle_pots() {
  // Break out if we have checked too recently
  if (pot_poll_interval.check() != 1) {
    return;
  }

  if (ENABLE_POT_DEBUGGING) {
    for (int i=14; i<=17; i++) {
      int val = analogRead(i);
      Serial.print("analogInput=");
      Serial.print(i, DEC);
      Serial.print(" value=");
      Serial.println(val);
    }
  }
}

/**
 * Listen to the pins and see what notes have been pressed, if any.
 */
void handle_notes() {
  // Break out if we have checked too recently
  if (pin_poll_interval.check() != 1) {
    return;
  }

  // Check the debouncers
  for (int i=0; i<LENGTH_PIN_NOTES; i++) {
    // Get the note that corresponds to the debouncer
    unsigned char note = pin_notes[i][1];

    // Get the debouncer and update its state
    Bounce *debouncer = &debouncers[i];
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

void loop() {
  handle_notes();
  handle_pots();
}
