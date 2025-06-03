#pragma once
#include <PCF8574outputs.h>
#include <Wire.h>

template <uint8_t IC_PIN_COUNT, uint8_t COUNT, uint8_t... ADDRESSES>
PCF8574outputs<IC_PIN_COUNT, COUNT, ADDRESSES...>::PCF8574outputs() {}

template <uint8_t IC_PIN_COUNT, uint8_t COUNT, uint8_t... ADDRESSES>
PCF8574outputs<IC_PIN_COUNT, COUNT, ADDRESSES...>::~PCF8574outputs() {}

template <uint8_t IC_PIN_COUNT, uint8_t COUNT, uint8_t... ADDRESSES>
bool PCF8574outputs<IC_PIN_COUNT, COUNT, ADDRESSES...>::begin(TwoWire *pWire, uint8_t reverse) {
  constexpr uint8_t addresses[COUNT] = {ADDRESSES...};  // Array of I2C addresses
  for (uint8_t i = 0; i < COUNT; ++i) {
    if (!_pcf[i].begin(pWire, addresses[i], PCF8574_MODE_OUT, reverse)) {
      return false;  // Return false if any device fails to initialize
    }
  }
  return true;
}

template <uint8_t IC_PIN_COUNT, uint8_t COUNT, uint8_t... ADDRESSES>
bool PCF8574outputs<IC_PIN_COUNT, COUNT, ADDRESSES...>::digitalWrite(uint8_t pin, bool value) {
  if (pin == 0 || pin > COUNT * IC_PIN_COUNT) return false;  // Validate pin number
  uint8_t chip = (pin - 1) / IC_PIN_COUNT;                   // Calculate which PCF8574 device
  uint8_t localPin = (pin - 1) % IC_PIN_COUNT;               // Calculate pin within the device
  return _pcf[chip].setValue(localPin, value);               // Set pin value
}

template <uint8_t IC_PIN_COUNT, uint8_t COUNT, uint8_t... ADDRESSES>
bool PCF8574outputs<IC_PIN_COUNT, COUNT, ADDRESSES...>::getOutputValue(uint8_t pin) {
  if (pin == 0 || pin > COUNT * IC_PIN_COUNT) return false;  // Validate pin number
  uint8_t chip = (pin - 1) / IC_PIN_COUNT;                   // Calculate which PCF8574 device
  uint8_t localPin = (pin - 1) % IC_PIN_COUNT;               // Calculate pin within the device
  return _pcf[chip].getValue(localPin);                      // Get pin value
}

template <uint8_t IC_PIN_COUNT, uint8_t COUNT, uint8_t... ADDRESSES>
bool PCF8574outputs<IC_PIN_COUNT, COUNT, ADDRESSES...>::writeBuffer(uint8_t chipIndex, uint8_t value) {
  if (chipIndex >= COUNT) return false;     // Validate chip index
  return _pcf[chipIndex].setValues(value);  // Write byte to device
}

template <uint8_t IC_PIN_COUNT, uint8_t COUNT, uint8_t... ADDRESSES>
uint8_t PCF8574outputs<IC_PIN_COUNT, COUNT, ADDRESSES...>::getBuffer(uint8_t chipIndex) {
  if (chipIndex >= COUNT) return 0;    // Validate chip index
  return _pcf[chipIndex].getValues();  // Get buffer value
}