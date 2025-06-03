# PCF8574 Arduino Library

This library provides an interface for controlling PCF8574 I2C I/O expander chips on Arduino platforms. It supports both input and output modes, with advanced event handling for inputs (e.g., clicks, double-clicks, holds) and flexible control of multiple devices.

## Features
- Control single or multiple PCF8574 I/O expanders.
- Input mode with event detection (click, double-click, hold, etc.).
- Output mode for setting individual pins or entire buffers.
- Logic inversion (reverse) option for pin states.
- Thread-safe operations with mutex (if supported).
- Template-based classes for managing multiple devices with different I2C addresses.
- Callback support for handling input events.

## Requirements
- Arduino IDE or compatible platform.
- `Wire` library for I2C communication.
- `Logger` library for error logging.
- `InputBase.h` and `OutputBase.h` for base class functionality (event handling and output control).

## Installation
1. Clone or download this repository.
2. Copy the library folder to your Arduino `libraries` directory.
3. Include the necessary headers in your sketch (e.g., `PCF8574.h`, `PCF8574inputs.h`, `PCF8574outputs.h`).

## Usage
### Single PCF8574 (Input Mode)
```cpp
#include <Wire.h>
#include <PCF8574.h>

PCF8574 pcf;
void setup() {
  Wire.begin();
  if (!pcf.begin(&Wire, 0x20, PCF8574_MODE_IN)) {
    Serial.println("PCF8574 initialization failed");
  }
}
void loop() {
  if (pcf.readBuffer()) {
    Serial.println(pcf.getValues(), BIN); // Print pin states
  }
  delay(100);
}
```

### Multiple PCF8574 Devices with Event Handling
This example uses two input and two output PCF8574 devices, with event detection and callbacks to toggle outputs on click events.
```cpp
#include <Arduino.h>
#include <Logger.h>
#include <PCF8574inputs.h>
#include <PCF8574outputs.h>
#include <Wire.h>

// Event name mapping
char *getName(uint16_t ev) {
  switch (ev) {
    case EVENT_CLICK: return (char *)"EVENT_CLICK";
    default: return (char *)"UNKNOWN_EVENT";
  }
}

PCF8574inputs<8, 2, 0x22, 0x21> inputs;
PCF8574outputs<8, 2, 0x24, 0x25> outputs;

void setup() {
  Serial.begin(115200);
  logger.begin(Serial);
  logger.setLogLevel(Loglevel::debug);
  Wire.begin(4, 5); // SDA on pin 4, SCL on pin 5
  inputs.begin(&Wire, true); // Logic inversion
  outputs.begin(&Wire, true);
  inputs.setEventsEnabledForAllPins(1023); // Enable events for all pins
  inputs.setCallbackForAllPins(inputCallback([](uint8_t pin, uint16_t pinType, uint16_t event, uint8_t holdCount) {
    logger.info("Pin: %i Event: %s", pin, getName(event));
    if (event == EVENT_CLICK) {
      outputs.digitalWrite(pin, !outputs.getOutputValue(pin)); // Toggle output
    }
  }));
}

void loop() {
  inputs.loop(); // Process input events
}
```

## Files
- `PCF8574.h` / `PCF8574.cpp`: Core class for a single PCF8574 device.
- `PCF8574inputs.h` / `PCF8574inputs.tpp`: Template class for multiple PCF8574 devices in input mode with event handling.
- `PCF8574outputs.h` / `PCF8574outputs.tpp`: Template class for multiple PCF8574 devices in output mode.

## License
This library is released under the MIT License. See the `LICENSE` file for details.