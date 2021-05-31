/* Create a "class compliant " USB to 3 MIDI IN and 3 MIDI OUT interface.

   MIDI receive (6N138 optocoupler) input circuit and series resistor
   outputs need to be connected to Serial1, Serial2 and Serial3.

   You must select MIDIx4 from the "Tools > USB Type" menu

   This example code is in the public domain.
*/

#include <MIDI.h>
#include "config.h"

typedef midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> HardwareSerialMidiInterface;

// Create the Serial MIDI ports
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI2);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI3);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial4, MIDI4);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial5, MIDI5);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial6, MIDI6);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial7, MIDI7);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial8, MIDI8);

// Put pointers to the MIDI interfaces into an array so we can loop more easily
#define LENGTH_MIDI_INTERFACES 8 
HardwareSerialMidiInterface *midi_interfaces[LENGTH_MIDI_INTERFACES] = {
  &MIDI1, &MIDI2, &MIDI3, &MIDI4, &MIDI5, &MIDI6, &MIDI7, &MIDI8
};

// A variable to know how long the LED has been turned on
elapsedMillis led_on_millis;
int is_activity;

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

    // Note: Display A doesn't use this pin when debugging
    pinMode(DEBUG_LED_PIN, OUTPUT); // LED pin
  }

  // Enable the MIDI serial ports 
  for (int i=0; i<LENGTH_MIDI_INTERFACES; i++) {
    HardwareSerialMidiInterface *midi_interface = midi_interfaces[i];
    midi_interface->begin(MIDI_CHANNEL_OMNI);
    midi_interface->turnThruOff();
  }

  if (ENABLE_DEBUGGING) {
    // Blink the LED
    digitalWriteFast(DEBUG_LED_PIN, HIGH); // LED on
    delay(HALF_SECOND_MILLIS);
    digitalWriteFast(DEBUG_LED_PIN, LOW);
  }
}

/**
 * Show the activity for MIDI
 */
void show_midi_activity(int is_activity) {
    // blink the LED when any activity has happened
  if (is_activity != 0) {
    // LED on
    digitalWriteFast(DEBUG_LED_PIN, HIGH);
    led_on_millis = 0;
  }
  if (led_on_millis > 15) {
    // LED off
    digitalWriteFast(DEBUG_LED_PIN, LOW);
  }
}

/**
 * Loop through the MIDI devices and do all the processing
 */
int process_midi() {
  int is_activity = 0;

  // Enable the MIDI serial ports 
  for (int i=0; i<LENGTH_MIDI_INTERFACES; i++) {
    // The virtual MIDI cable over MIDI, numbered 0-15
    byte usb_midi_cable_number = i;
    // The midi channel is sent as 1-16 in this library
    midi::Channel hardware_output_midi_channel = i+1;
    
    HardwareSerialMidiInterface *midi_interface = midi_interfaces[i];
    if (midi_interface->read()) {
      // get a MIDI IN (Serial) message
      midi::MidiType type = midi_interface->getType();
      midi::Channel channel = midi_interface->getChannel();
      midi::DataByte data1 = midi_interface->getData1();
      midi::DataByte data2 = midi_interface->getData2();

      // We typically override the input channel so that it's predictable
      // regardless of the channel set on the input keyboard
      midi::Channel usb_output_midi_channel = channel;
      if (USB_MIDI_CHANNEL > 0) {
        usb_output_midi_channel = USB_MIDI_CHANNEL;
      }
  
      // forward the message to USB MIDI virtual cable #0
      if (type != midi::SystemExclusive) {
        // Whatever comes to the first hardware serial port (number 1) goes 
        // to the first cable (number 0), and so forth. The output channel is always
        // set to USB_MIDI_CHANNEL (1), instead of using the channel that the keyboard
        // sent it on.
        usbMIDI.send(type, data1, data2, usb_output_midi_channel, usb_midi_cable_number);
 
        // For ease of use with sound generators that only have an input MIDI port,
        // we concatenate all the messages onto one hardware output, but we make it so that
        // everything coming coming into first hardware serial port (number 1) goes
        // to channel 1, second hardware serial port goes to channel 2, etc.
        // So this won't work well in any cases (probabably rare?) where you have a keyboard
        // which needs to send messages on more than one channel. In that case, just
        // use the USB MIDI instead.
        // Use MIDI8 as OUTPUT_MIDI_CONCATENATED_PORT
        MIDI8.send(type, data1, data2, hardware_output_midi_channel);
      
      } else {

        // SysEx messages are special.  The message length is given in data1 & data2
        unsigned int sysex_length = data1 + data2 * 256;
        const byte *sysex_array_ptr = midi_interface->getSysExArray();

        // Send the SysEx over MIDI
        usbMIDI.sendSysEx(sysex_length, sysex_array_ptr, true, usb_midi_cable_number);

        // Put all the SysEx from all the inputs to the one hardware output
        // Use MIDI8 as OUTPUT_MIDI_CONCATENATED_PORT
        MIDI8.sendSysEx(sysex_length, sysex_array_ptr, true);
      }

      is_activity = 1;
    }
  }

  if (usbMIDI.read()) {
    // get the USB MIDI message, defined by these 5 numbers (except SysEX)
    byte type = usbMIDI.getType();
    byte channel = usbMIDI.getChannel();
    byte data1 = usbMIDI.getData1();
    byte data2 = usbMIDI.getData2();
    byte cable = usbMIDI.getCable();

    // Whatever cable the info came in on, send it out on the corresponding output
    // NOTE: We don't support MIDI8 because we are using that as our
    // OUTPUT_MIDI_CONCATENATED_PORT.
    // If a message comes in on a cable we don't support as an output, drop the message.
    const byte max_input_cable_index = LENGTH_MIDI_INTERFACES - 2;
    if (cable < 0 || cable > max_input_cable_index) {
      return is_activity;
    }

    // Get the corresponding midi interface to the incoming cable
    HardwareSerialMidiInterface *midi_interface = midi_interfaces[cable];

    // forward this message to 1 of the 3 Serial MIDI OUT ports
    if (type != usbMIDI.SystemExclusive) {
      // Normal messages, first we must convert usbMIDI's type (an ordinary
      // byte) to the MIDI library's special MidiType.
      midi::MidiType mtype = (midi::MidiType)type;
      midi_interface->send(mtype, data1, data2, channel);

    } else {
      // SysEx messages are special.  The message length is given in data1 & data2
      unsigned int sysex_length = data1 + data2 * 256;
      midi_interface->sendSysEx(sysex_length, usbMIDI.getSysExArray(), true);
    }
    is_activity = 1;
  }

  return is_activity;
}

/**
 * Main program loop
 */
void loop() {
  int is_activity = process_midi();

  if (ENABLE_DEBUGGING) {
    show_midi_activity(is_activity);
  }
}
