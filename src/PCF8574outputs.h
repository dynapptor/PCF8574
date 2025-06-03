#pragma once
#include <OutputBase.h>

#include "PCF8574.h"

class TwoWire;  // Forward declaration of TwoWire class

/**
 * @file PCF8574outputs.h
 * @brief Template class for managing multiple PCF8574 devices in output mode.
 * @author [Your Name]
 * @date 2025-06-03
 */

/**
 * @class PCF8574outputs
 * @brief Manages multiple PCF8574 I/O expanders in output mode.
 * @tparam IC_PIN_COUNT Number of pins per PCF8574 (typically 8).
 * @tparam COUNT Number of PCF8574 devices.
 * @tparam ADDRESSES I2C addresses of the devices.
 */
template <uint8_t IC_PIN_COUNT, uint8_t COUNT, uint8_t... ADDRESSES>
class PCF8574outputs : public OutputBase<IC_PIN_COUNT * COUNT> {
  static_assert(sizeof...(ADDRESSES) == COUNT, "COUNT must match number of ADDRESSES");  ///< Ensure address count matches COUNT.

 public:
  /**
   * @brief Constructor, initializes default state.
   */
  PCF8574outputs();

  /**
   * @brief Destructor, cleans up resources.
   */
  ~PCF8574outputs();

  /**
   * @brief Initializes all PCF8574 devices in output mode.
   * @param pWire Pointer to TwoWire object for I2C communication.
   * @param reverse If true, inverts pin logic (default: 0).
   * @return bool True if initialization is successful, false otherwise.
   */
  bool begin(TwoWire *pWire, uint8_t reverse = 0);

  /**
   * @brief Sets the state of a specific pin.
   * @param pin Pin number (1 to IC_PIN_COUNT * COUNT).
   * @param value True for high, false for low.
   * @return bool True if write is successful, false otherwise.
   */
  bool digitalWrite(uint8_t pin, bool value) override;

  /**
   * @brief Gets the current output state of a specific pin.
   * @param pin Pin number (1 to IC_PIN_COUNT * COUNT).
   * @return bool True if pin is high, false if low.
   */
  bool getOutputValue(uint8_t pin) override;

  /**
   * @brief Writes a byte to the buffer of a specific PCF8574 device.
   * @param chipIndex Index of the PCF8574 device (0 to COUNT-1).
   * @param value Byte to write.
   * @return bool True if write is successful, false otherwise.
   */
  bool writeBuffer(uint8_t chipIndex, uint8_t value);

  /**
   * @brief Gets the current buffer value of a specific PCF8574 device.
   * @param chipIndex Index of the PCF8574 device (0 to COUNT-1).
   * @return uint8_t Current buffer value.
   */
  uint8_t getBuffer(uint8_t chipIndex);

 private:
  PCF8574 _pcf[COUNT];  ///< Array of PCF8574 objects.
};

#include "PCF8574outputs.tpp"