# PCF8574 Arduino Library API

This document describes the API for the PCF8574 Arduino library, which provides interfaces for controlling PCF8574 I2C I/O expander chips with input event handling and output control.

## Classes

### PCF8574
Manages a single PCF8574 I/O expander.

#### Methods
- `PCF8574()`: Constructor.
- `~PCF8574()`: Destructor.
- `bool begin(TwoWire *pWire, uint8_t address, uint8_t mode = PCF8574_MODE_IN, uint8_t reverse = 0)`: Initializes the device.
  - `pWire`: Pointer to TwoWire object.
  - `address`: I2C address (0x20 to 0x27).
  - `mode`: `PCF8574_MODE_IN` (1) or `PCF8574_MODE_OUT` (0).
  - `reverse`: If true, inverts pin logic.
  - Returns: `true` if successful, `false` otherwise.
- `uint8_t getMode()`: Returns the current mode (1 for input, 0 for output).
- `uint8_t getReverse()`: Returns the reverse flag (1 if inverted, 0 otherwise).
- `bool readBuffer()`: Reads the current pin states (input mode only). Returns `true` if successful.
- `uint8_t getValues()`: Returns the current pin values as a byte.
- `uint8_t getLastValues()`: Returns the last read pin values as a byte.
- `bool getValue(uint8_t pin)`: Gets the state of a specific pin (0-7). Returns `true` if high, `false` if low.
- `bool getLastValue(uint8_t pin)`: Gets the last read state of a specific pin (0-7). Returns `true` if high, `false` if low.
- `bool setValues(uint8_t values)`: Sets all pin values (output mode only). Returns `true` if successful.
- `bool setValue(uint8_t pin, uint8_t value)`: Sets a specific pin (0-7) to a value (0 or 1). Returns `true` if successful.
- `uint32_t getLastRead()`: Returns the timestamp (in milliseconds) of the last read.

### PCF8574inputs<IC_PIN_COUNT, COUNT, ADDRESSES...>
Template class for managing multiple PCF8574 devices in input mode with event handling.

#### Template Parameters
- `IC_PIN_COUNT`: Number of pins per PCF8574 (typically 8).
- `COUNT`: Number of PCF8574 devices.
- `ADDRESSES`: I2C addresses of the devices.

#### Methods
- `PCF8574inputs()`: Constructor.
- `~PCF8574inputs()`: Destructor.
- `bool begin(TwoWire *pWire, uint8_t reverse = 0)`: Initializes all devices. Returns `true` if successful.
- `bool digitalRead(uint8_t pin)`: Reads the state of a pin (1 to `IC_PIN_COUNT * COUNT`). Returns `true` if high, `false` if low.
- `bool getLastValue(uint8_t pin)`: Gets the last read state of a pin (1 to `IC_PIN_COUNT * COUNT`). Returns `true` if high, `false` if low.
- `bool readBuffer()`: Reads the buffer of all devices. Returns `true` if successful.
- `void setEventsEnabledForAllPins(uint16_t bitmask)`: Enables event detection for pins based on a bitmask (inherited from `InputBase`).
  - `bitmask`: Bits set to 1 enable events for corresponding pins.
- `void setCallbackForAllPins(inputCallback callback)`: Sets a callback function for all pins to handle events (inherited from `InputBase`).
  - `callback`: Lambda or function with signature `void(uint8_t pin, uint16_t pinType, uint16_t event, uint8_t holdCount)`.
- `void loop()`: Processes input events and triggers callbacks (inherited from `InputBase`).

### PCF8574outputs<IC_PIN_COUNT, COUNT, ADDRESSES...>
Template class for managing multiple PCF8574 devices in output mode.

#### Template Parameters
- `IC_PIN_COUNT`: Number of pins per PCF8574 (typically 8).
- `COUNT`: Number of PCF8574 devices.
- `ADDRESSES`: I2C addresses of the devices.

#### Methods
- `PCF8574outputs()`: Constructor.
- `~PCF8574outputs()`: Destructor.
- `bool begin(TwoWire *pWire, uint8_t reverse = 0)`: Initializes all devices. Returns `true` if successful.
- `bool digitalWrite(uint8_t pin, bool value)`: Sets a pin (1 to `IC_PIN_COUNT * COUNT`) to a value (`true` for high, `false` for low). Returns `true` if successful.
- `bool getOutputValue(uint8_t pin)`: Gets the current state of a pin (1 to `IC_PIN_COUNT * COUNT`). Returns `true` if high, `false` if low.
- `bool writeBuffer(uint8_t chipIndex, uint8_t value)`: Writes a byte to a specific device (0 to `COUNT-1`). Returns `true` if successful.
- `uint8_t getBuffer(uint8_t chipIndex)`: Gets the current buffer value of a specific device (0 to `COUNT-1`).

## Constants
- `PCF8574_MODE_IN`: 1 (input mode).
- `PCF8574_MODE_OUT`: 0 (output mode).
- Event constants (defined in `InputBase` or similar):
  - `EVENT_NONE`, `EVENT_DOWN`, `EVENT_UP`, `EVENT_CLICK`, `EVENT_DOUBLE_DOWN`, `EVENT_DOUBLE_CLICK`, `EVENT_HOLD`, `EVENT_HOLD_UP`, `EVENT_DOUBLE_HOLD`, `EVENT_DOUBLE_HOLD_UP`.

## Notes
- Pin numbers for `PCF8574inputs` and `PCF8574outputs` start at 1, not 0.
- The `reverse` parameter inverts pin logic (e.g., high becomes low and vice versa).
- Ensure I2C addresses are valid (0x20 to 0x27) and unique.
- Event handling requires `InputBase` or a similar base class providing `setEventsEnabledForAllPins`, `setCallbackForAllPins`, and `loop`.