/**
 * Overload of Keypad library to override pin mode function.
 */
#include "PistonKeypad.h"

/**
 * Constructor. We do more pin initialization here than in the original library.
 */
PistonKeypad::PistonKeypad(char *userKeymap, byte *row, byte *col, byte numRows, byte numCols): Keypad(userKeymap, row, col, numRows, numCols) {
  // Initialize row pins
  for (byte r=0; r<numRows; r++) {
    Keypad::pin_mode(row[r],INPUT_PULLUP);
  }
  
  // Initialize column pins
  for (byte c=0; c<numCols; c++) {
    Keypad::pin_mode(col[c],INPUT);
  }
}

/**
 * Override of the pin_write virtual function.
 */
void PistonKeypad::pin_write(byte pinNum, boolean level) {
  // We skip writing the high pulse that happens during the library scanKeys()
  // method. When that was in there, I got spurious readings on the Teensy
  if (level != HIGH) {
    Keypad::pin_write(pinNum, level);
  }
}

/**
 * Override of the pin_mode virtual function.
 * We need to do a small wait after each transition of a output
 * pin to an input, since otherwise I was getting spurious readings.
 * Also, I change the output mode from OUTPUT to OUTPUT_OPENDRAIN.
 */
void PistonKeypad::pin_mode(byte pinNum, byte mode) {
  // In the current version of the Keypad library, INPUT is only called
  // at the end of a scan of a row, when an OUTPUT is being changed to an
  // INPUT, and right before the next row's INPUT is being changed to an
  // OUTPUT for scanning. This is where we need to put a delay.
  if (mode == INPUT) {
    Keypad::pin_mode(pinNum, INPUT_PULLDOWN);
    // Put in the delay.
    delayMicroseconds(100);

  } else if (mode == OUTPUT) {
    // We use OUTPUT_OPENDRAIN since Teensy supports it.
    // I don't know if this is absolutely necessary but we might as well,
    // since we are using this as an open collector / open drain.
    Keypad::pin_mode(pinNum, OUTPUT_OPENDRAIN);
  
  } else if (mode == INPUT_PULLUP) {
    // This is what the library sets the input pins to every time. However,
    // since we do this once in the constructor, we don't need to do this any time.
    // NOOP
    ;
  } else {
    // Let any other option pass through as usual
    Keypad::pin_mode(pinNum, mode);
  }
}
