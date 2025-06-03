#include <Arduino.h>
#include <Logger.h>
#include <PCF8574inputs.h>
#include <PCF8574outputs.h>
#include <Wire.h>

// Event name mapping
char *getName(uint16_t ev) {
  switch (ev) {
    case EVENT_NONE:
      return (char *)"EVENT_NONE";
    case EVENT_DOWN:
      return (char *)"EVENT_DOWN";
    case EVENT_UP:
      return (char *)"EVENT_UP";
    case EVENT_CLICK:
      return (char *)"EVENT_CLICK";
    case EVENT_DOUBLE_DOWN:
      return (char *)"EVENT_DOUBLE_DOWN";
    case EVENT_DOUBLE_CLICK:
      return (char *)"EVENT_DOUBLE_CLICK";
    case EVENT_HOLD:
      return (char *)"EVENT_HOLD";
    case EVENT_HOLD_UP:
      return (char *)"EVENT_HOLD_UP";
    case EVENT_DOUBLE_HOLD:
      return (char *)"EVENT_DOUBLE_HOLD";
    case EVENT_DOUBLE_HOLD_UP:
      return (char *)"EVENT_DOUBLE_HOLD_UP";
    default:
      return (char *)"UNKNOWN_EVENT";
  }
}

PCF8574inputs<8, 2, 0x22, 0x21> inputs;
PCF8574outputs<8, 2, 0x24, 0x25> outputs;

void setup() {
  Serial.begin(115200);
  logger.begin(Serial);
  logger.setLogLevel(Loglevel::debug);
  Wire.begin();           // SDA on pin 4, SCL on pin 5
  inputs.begin(&Wire, true);  // Logic inversion
  outputs.begin(&Wire, true);
  inputs.setEventsEnabledForAllPins(1023);  // Enable events for all pins
  inputs.setCallbackForAllPins(inputCallback([](uint8_t pin, uint16_t pinType, uint16_t event, uint8_t holdCount) {
    logger.info("Pin: %i Event: %s", pin, getName(event));
    if (event == EVENT_CLICK) {
      outputs.digitalWrite(pin, !outputs.getOutputValue(pin));  // Toggle output
    }
  }));
}

void loop() {
  inputs.loop();  // Process input events
}
