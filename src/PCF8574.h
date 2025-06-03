#pragma once
#include <Arduino.h>

/**
 * @file PCF8574.h
 * @brief Class for controlling a single PCF8574 I2C I/O expander on Arduino.
 * @author [Your Name]
 * @date 2025-06-03
 */

/**
 * @def PCF8574_MODE_IN
 * @brief Constant for PCF8574 input mode (value: 1).
 */
#define PCF8574_MODE_IN 1

/**
 * @def PCF8574_MODE_OUT
 * @brief Constant for PCF8574 output mode (value: 0).
 */
#define PCF8574_MODE_OUT 0

class TwoWire;  // Forward declaration of TwoWire class

/**
 * @class PCF8574
 * @brief Manages a single PCF8574 I2C I/O expander for input or output operations.
 */
class PCF8574 {
 public:
  /**
   * @brief Constructor, initializes default state.
   */
  PCF8574();

  /**
   * @brief Destructor, cleans up resources.
   */
  ~PCF8574();

  /**
   * @brief Initializes the PCF8574 with I2C wire, address, mode, and reverse flag.
   * @param pWire Pointer to TwoWire object for I2C communication.
   * @param address I2C address of the device (0x20 to 0x27).
   * @param mode Operating mode (PCF8574_MODE_IN or PCF8574_MODE_OUT).
   * @param reverse If true, inverts pin logic (default: 0).
   * @return bool True if initialization is successful, false otherwise.
   */
  bool begin(TwoWire *pWire, uint8_t address, uint8_t mode = PCF8574_MODE_IN, uint8_t reverse = 0);

  /**
   * @brief Gets the current operating mode.
   * @return uint8_t 1 for input mode, 0 for output mode.
   */
  uint8_t getMode();

  /**
   * @brief Gets the reverse logic flag.
   * @return uint8_t 1 if pin logic is inverted, 0 otherwise.
   */
  uint8_t getReverse();

  /**
   * @brief Reads the current pin states into the buffer (input mode only).
   * @return bool True if read is successful, false otherwise.
   */
  bool readBuffer();

  /**
   * @brief Gets the current pin values as a byte.
   * @return uint8_t Byte representing the state of all pins.
   */
  uint8_t getValues();

  /**
   * @brief Gets the last read pin values as a byte.
   * @return uint8_t Byte representing the last read state of all pins.
   */
  uint8_t getLastValues();

  /**
   * @brief Gets the value of a specific pin.
   * @param pin Pin number (0-7).
   * @return bool True if pin is high, false if low.
   */
  bool getValue(uint8_t pin);

  /**
   * @brief Gets the last read value of a specific pin.
   * @param pin Pin number (0-7).
   * @return bool True if pin was high, false if low.
   */
  bool getLastValue(uint8_t pin);

  /**
   * @brief Sets all pin values at once (output mode only).
   * @param values Byte representing the state of all pins.
   * @return bool True if write is successful, false otherwise.
   */
  bool setValues(uint8_t values);

  /**
   * @brief Sets the value of a specific pin (output mode only).
   * @param pin Pin number (0-7).
   * @param value 0 for low, 1 for high.
   * @return bool True if write is successful, false otherwise.
   */
  bool setValue(uint8_t pin, uint8_t value);

  /**
   * @brief Gets the timestamp of the last read operation.
   * @return uint32_t Timestamp in milliseconds.
   */
  uint32_t getLastRead();

 private:
  TwoWire *_wire = nullptr;  ///< Pointer to I2C wire object.
  uint8_t _address = 0;      ///< I2C address of the device.
  uint8_t _mode = 1;         ///< Current mode (input or output).
  uint32_t _lastRead = 0;    ///< Timestamp of last read operation.
  uint8_t _values = 0;       ///< Current pin values.
  uint8_t _lastValues = 0;   ///< Last read pin values.
  bool _reverse = 0;         ///< Reverse logic flag.
};