#pragma once
#include <Logger.h>
#include <Wire.h>

#include "PCF8574inputs.h"

template <uint8_t IC_PIN_COUNT, uint8_t COUNT, uint8_t... ADDRESSES>
PCF8574inputs<IC_PIN_COUNT, COUNT, ADDRESSES...>::PCF8574inputs() {}

template <uint8_t IC_PIN_COUNT, uint8_t COUNT, uint8_t... ADDRESSES>
PCF8574inputs<IC_PIN_COUNT, COUNT, ADDRESSES...>::~PCF8574inputs() {}

template <uint8_t IC_PIN_COUNT, uint8_t COUNT, uint8_t... ADDRESSES>
bool PCF8574inputs<IC_PIN_COUNT, COUNT, ADDRESSES...>::begin(TwoWire *pWire, uint8_t reverse) {
  constexpr uint8_t addresses[COUNT] = {ADDRESSES...};  // Array of I2C addresses
  for (size_t i = 0; i < COUNT; i++) {
    if (!_pcf[i].begin(pWire, addresses[i], PCF8574_MODE_IN, reverse)) {
      logger.error(F("PC857x at 0x%02X address connect failed, exiting..."), addresses[i]);  // Log error if initialization fails
      return false;
    }
  }
  return true;
}

template <uint8_t IC_PIN_COUNT, uint8_t COUNT, uint8_t... ADDRESSES>
bool PCF8574inputs<IC_PIN_COUNT, COUNT, ADDRESSES...>::digitalRead(uint8_t pin) {
  if (pin == 0 || pin > IC_PIN_COUNT * COUNT) return false;  // Validate pin number
  int pcfIndex = (pin - 1) / IC_PIN_COUNT;                   // Calculate which PCF8574 device
  int pcfPin = (pin - 1) % IC_PIN_COUNT;                     // Calculate pin within the device
  return _pcf[pcfIndex].getValue(pcfPin);                    // Read pin value
}

template <uint8_t IC_PIN_COUNT, uint8_t COUNT, uint8_t... ADDRESSES>
bool PCF8574inputs<IC_PIN_COUNT, COUNT, ADDRESSES...>::getLastValue(uint8_t pin) {
  if (pin == 0 || pin > IC_PIN_COUNT * COUNT) return false;  // Validate pin number
  int pcfIndex = (pin - 1) / IC_PIN_COUNT;                   // Calculate which PCF8574 device
  int pcfPin = (pin - 1) % IC_PIN_COUNT;                     // Calculate pin within the device
  return _pcf[pcfIndex].getLastValue(pcfPin);                // Read last pin value
}

template <uint8_t IC_PIN_COUNT, uint8_t COUNT, uint8_t... ADDRESSES>
bool PCF8574inputs<IC_PIN_COUNT, COUNT, ADDRESSES...>::readBuffer() {
  for (size_t i = 0; i < COUNT; i++) {
    if (!_pcf[i].readBuffer()) {
      logger.error(F("readBuffer() failed, exiting..."));  // Log error if read fails
      return false;
    }
  }
  return true;
}