#ifndef PISTONKEYPAD_H
#define PISTONKEYPAD_H

#include <Keypad.h>

/**
 * Overload of Keypad library to override pin mode function.
 */
class PistonKeypad : public Keypad {
public:
  /**
   * Constructor
   */
  PistonKeypad(char *userKeymap, byte *row, byte *col, byte numRows, byte numCols);

  /**
   * Virtual function overrides
   */
  virtual void pin_mode(byte pinNum, byte mode);
  virtual void pin_write(byte pinNum, boolean level);
};

#endif
