#include "PCF8574.h"

#include <Wire.h>

PCF8574::PCF8574() {}

PCF8574::~PCF8574() {}

bool PCF8574::begin(TwoWire *pWire, uint8_t address, uint8_t mode, uint8_t reverse) {
  _reverse = reverse;  // Store reverse flag for logic inversion
  if (!pWire) {
    return false;  // Fail if no valid I2C wire is provided
  }
  _wire = pWire;                                      // Store I2C wire pointer
  _address = address;                                 // Store device I2C address
  _mode = mode ? PCF8574_MODE_IN : PCF8574_MODE_OUT;  // Set mode (input or output)
  _wire->beginTransmission(_address);                 // Start I2C transmission to device
  if (_mode) {
    _wire->write(0xFF);  // In input mode, set all pins high (pull-up resistors)
  } else {
    _values = 0x00;                               // In output mode, initialize all pins to low
    _lastValues = 0x00;                           // Initialize last values to low
    _wire->write(_reverse ? ~_values : _values);  // Write initial values, inverted if reverse is true
  }
  bool success = (_wire->endTransmission() == 0);  // Check if I2C transmission was successful
  if (success && _mode) {
    return readBuffer();  // In input mode, read initial pin states
  }
  return success;  // Return initialization result
};

uint8_t PCF8574::getMode() { return _mode; }

uint8_t PCF8574::getReverse() { return _reverse; }

uint32_t PCF8574::getLastRead() {
  return _lastRead;
}

bool PCF8574::readBuffer() {
  if ((!_wire) || (_mode == 0)) return false;  // Validate wire and input mode
  _wire->requestFrom(_address, (uint8_t)1);    // Request 1 byte from the device
  _lastRead = millis();                        // Record the time of the read operation
  _lastValues = _values;                       // Save current values as last values
  if (_wire->available()) {
    uint8_t v = _wire->read();    // Read the byte from I2C
    _values = _reverse ? ~v : v;  // Store value, inverting if reverse is true
    return true;                  // Read successful
  }
  return false;  // Read failed (no data available)
}

uint8_t PCF8574::getValues() { return _values; }

uint8_t PCF8574::getLastValues() { return _lastValues; }

bool PCF8574::getValue(uint8_t pin) {
  return bitRead(_values, pin);  // Extract bit for specified pin
}

bool PCF8574::getLastValue(uint8_t pin) {
  return bitRead(_lastValues, pin);  // Extract bit for specified pin from last values
}

bool PCF8574::setValues(uint8_t values) {
  if ((!_wire) || (_mode == 1)) return false;      // Validate wire and output mode
  _wire->beginTransmission(_address);              // Start I2C transmission
  _wire->write(_reverse ? ~values : values);       // Write values, inverted if reverse is true
  bool success = (_wire->endTransmission() == 0);  // Check if transmission was successful
  return success;                                  // Return write result
}

bool PCF8574::setValue(uint8_t pin, uint8_t value) {
  if (!_wire) {
    return false;  // Fail if no valid I2C wire
  }
  bitWrite(_values, pin, value);                   // Update the specified pin in the values byte
  _wire->beginTransmission(_address);              // Start I2C transmission
  _wire->write(_reverse ? ~_values : _values);     // Write updated values, inverted if reverse is true
  bool success = (_wire->endTransmission() == 0);  // Check if transmission was successful
  return success;                                  // Return write result
}