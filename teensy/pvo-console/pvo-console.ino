
/**
 * Handles midi routing for four MIDI keyboards, plus a 9x12 matrix of piston buttons.
 */
#include <MIDI.h>
#include "config.h"
#include "PistonKeypad.h"

//// Poll intervals
//#include <Metro.h>
//Metro midi_poll_interval = Metro(1);
//Metro keypad_poll_interval = Metro(20);

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

// Set up the keypad
PistonKeypad kpd = PistonKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

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

  // Configure the keypad
  kpd.setDebounceTime(DEBOUNCE_INTERVAL_MILLIS);
  kpd.setHoldTime(HOLD_INTERVAL_MILLIS);

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
//  // Break out if we have checked too recently
//  if (midi_poll_interval.check() != 1) {
//    return;
//  }

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
      if (type == midi::SystemExclusive) {
        // We discard ActiveSensing
        ;
      } else if (type != midi::SystemExclusive) {
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

    if (type == usbMIDI.ActiveSensing) {
      // We discard ActiveSensing
      ;
    } else if (type != usbMIDI.SystemExclusive) {
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

void process_keypad() {
//  if (keypad_poll_interval.check() != 1) {
//    return;
//  }
  // Returns true if there are ANY active keys.
  if (!kpd.getKeys()) {
    return;
  }

  // Scan the whole key list, since we know there is an active key
  for (int i=0; i<LIST_MAX; i++) {
    Key *current_key = &kpd.key[i];
 
    // Only find keys that have changed state.
    if ( current_key->stateChanged ) {
      // The name of the key
      char current_key_name = current_key->kchar;
      KeyState current_key_state = current_key->kstate;

      // On USB cables, we will send out the message on channel 1
      midi::Channel usb_output_midi_channel = 1;

      // The midi message to send depends on if it's a button
      // which needs to be held down.
      if (current_key_name == SET_BUTTON_NUMBER) {
        // We need to send this as as a CC since it has to show when
        // it is both pressed and released
        if (current_key_state == PRESSED || current_key_state == RELEASED) {
          char button_cc_value = current_key_state == PRESSED ? 127 : 0;
          
          // Send the message over the PISTON_MSG_CABLE_NUM virtual MIDI cable on USB
          usbMIDI.sendControlChange(current_key_name, button_cc_value,
            usb_output_midi_channel, PISTON_MSG_CABLE_NUM);
          
          // Use MIDI8 as OUTPUT_MIDI_CONCATENATED_PORT.
          // Send button messages on PISTON_MSG_CONCAT_PORT_CHANNEL
          MIDI8.sendControlChange(current_key_name, button_cc_value, PISTON_MSG_CONCAT_PORT_CHANNEL);
        }
 
      } else {
        // Since this is simply a momentary button, we will send it as a program change
        // when the button is first pushed
        if (current_key_state == PRESSED) {
          usbMIDI.sendProgramChange(current_key_name, usb_output_midi_channel, PISTON_MSG_CABLE_NUM);
          MIDI8.sendProgramChange(current_key_name, PISTON_MSG_CONCAT_PORT_CHANNEL);
        }
      }

      if (ENABLE_DEBUGGING) {
        switch (current_key_state) {
          case PRESSED:
            Serial.print("pressed: ");
            Serial.println(current_key_name, DEC);
            break;
          case RELEASED:
            Serial.print("released: ");
            Serial.println(current_key_name, DEC);
            break;
          case HOLD:
            Serial.print("hold: ");
            Serial.println(current_key_name, DEC);
            break;
          case IDLE:
            Serial.print("idle: ");
            Serial.println(current_key_name, DEC);
            break;
          default:
            // fall through
            break;
        } 
      }
    }
  }
}

/**
 * Main program loop
 */
void loop() {
  int is_activity = process_midi();

  process_keypad();

  if (ENABLE_DEBUGGING) {
    show_midi_activity(is_activity);
  }
}
