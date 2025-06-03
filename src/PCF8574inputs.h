#pragma once
#include <InputBase.h>

#include "PCF8574.h"

class TwoWire;  // Forward declaration of TwoWire class

/**
 * @file PCF8574inputs.h
 * @brief Template class for managing multiple PCF8574 devices in input mode.
 * @author [Your Name]
 * @date 2025-06-03
 */

/**
 * @class PCF8574inputs
 * @brief Manages multiple PCF8574 I/O expanders in input mode with event handling.
 * @tparam IC_PIN_COUNT Number of pins per PCF8574 (typically 8).
 * @tparam COUNT Number of PCF8574 devices.
 * @tparam ADDRESSES I2C addresses of the devices.
 */
template <uint8_t IC_PIN_COUNT, uint8_t COUNT, uint8_t... ADDRESSES>
class PCF8574inputs : public InputBase<IC_PIN_COUNT * COUNT> {
  static_assert(sizeof...(ADDRESSES) == COUNT, "COUNT must match number of ADDRESSES");  ///< Ensure address count matches COUNT.

 public:
  /**
   * @brief Constructor, initializes default state.
   */
  PCF8574inputs();

  /**
   * @brief Destructor, cleans up resources.
   */
  ~PCF8574inputs();

  /**
   * @brief Initializes all PCF8574 devices in input mode.
   * @param pWire Pointer to TwoWire object for I2C communication.
   * @param reverse If true, inverts pin logic (default: 0).
   * @return bool True if initialization is successful, false otherwise.
   */
  bool begin(TwoWire *pWire, uint8_t reverse = 0);

  /**
   * @brief Reads the state of a specific pin.
   * @param pin Pin number (1 to IC_PIN_COUNT * COUNT).
   * @return bool True if pin is high, false if low.
   */
  bool digitalRead(uint8_t pin) override;

  /**
   * @brief Gets the last read state of a specific pin.
   * @param pin Pin number (1 to IC_PIN_COUNT * COUNT).
   * @return bool True if pin was high, false if low.
   */
  bool getLastValue(uint8_t pin) override;

  /**
   * @brief Reads the buffer of all PCF8574 devices.
   * @return bool True if read is successful, false otherwise.
   */
  bool readBuffer() override;

 private:
  PCF8574 _pcf[COUNT];  ///< Array of PCF8574 objects.
};

#include "PCF8574inputs.tpp"