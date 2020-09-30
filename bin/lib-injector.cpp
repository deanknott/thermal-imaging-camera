
// DON'T EDIT FROM HERE TO "END"
// (this stuff is injected to support unPhone's TCA9555 IO expander chip)
#include <Wire.h>  // (we need digitalRead etc. defined before redefining)
class IOExpander { // this is an excerpt of the full definition that...
public:            // ...just declares the injected methods
  static void pinMode(uint8_t pin, uint8_t mode);
  static void digitalWrite(uint8_t pin, uint8_t value);
  static uint8_t digitalRead(uint8_t pin);
};
#define digitalWrite IOExpander::digitalWrite // call...
#define digitalRead  IOExpander::digitalRead  // ...ours...
#define pinMode      IOExpander::pinMode      // ...please
// END -- normal library code follows

