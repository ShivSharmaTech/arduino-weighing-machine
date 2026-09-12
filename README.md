# Arduino 50 kg Weighing Machine

An Arduino Nano based digital weighing machine using an **HX711 load-cell amplifier** and a **6-digit common-cathode 7-segment display**.

The project is organized into separate modules so that the display, buttons, calibration, EEPROM, and main weighing logic can be managed and modified independently.

## Features

* Weighing capacity: **50 kg / 50,000 g**
* Arduino Nano based
* HX711 load-cell amplifier
* 6-digit 7-segment display
* Weight displayed in grams
* ZERO / TARE function
* Calibration through buttons
* Calibration value stored in EEPROM
* Calibration value automatically loaded after restart
* Negative weight display
* Digit-by-digit calibration editing
* Selected calibration digit blinking
* Modular and maintainable code structure

## Project Structure

```text
WeighingMachine/
│
├── WeighingMachine.ino      # Main program and weighing logic
│
├── Display.h                # Display declarations
├── Display.cpp              # 7-segment display functions
│
├── Buttons.h                # Button declarations
├── Buttons.cpp              # Button input handling
│
├── Calibration.h            # Calibration declarations
├── Calibration.cpp          # Calibration process
│
├── EEPROMData.h             # EEPROM declarations
└── EEPROMData.cpp           # EEPROM read/write functions
```

## Hardware

### Main Components

* Arduino Nano
* 50 kg Load Cell
* HX711 Load Cell Amplifier
* 6-digit Common-Cathode 7-Segment Display
* CAL button
* UP button
* SHIFT button
* ZERO button
* External 10k pull-up resistors for CAL and UP buttons

## Pin Configuration

### HX711

| HX711 | Arduino Nano |
| ----- | ------------ |
| DOUT  | D13          |
| SCK   | D12          |

### 7-Segment Display

| Segment | Arduino Nano |
| ------- | -----------: |
| A       |           D2 |
| B       |           D3 |
| C       |           D4 |
| D       |           D5 |
| E       |           D6 |
| F       |           D7 |
| G       |           D8 |
| DP      |           D9 |

### Digit Commons

The digit numbering is **right-to-left**.

| Display Digit | Position  | Arduino Pin |
| ------------- | --------- | ----------: |
| Digit 1       | Rightmost |         D10 |
| Digit 2       |           |         D11 |
| Digit 3       |           |          A0 |
| Digit 4       |           |          A1 |
| Digit 5       |           |          A2 |
| Digit 6       | Leftmost  |          A3 |

### Buttons

| Button | Arduino Pin |
| ------ | ----------: |
| CAL    |          A6 |
| UP     |          A7 |
| SHIFT  |          A4 |
| ZERO   |          A5 |

> CAL and UP use external pull-up resistors.
> SHIFT and ZERO use the Arduino's internal pull-up resistors.

## Display Configuration

The display is a **common-cathode** type.

* Segment `HIGH` = ON
* Digit `LOW` = ON
* Digit `HIGH` = OFF

The project uses the following segment patterns:

```text
0 = B00111111
1 = B00000110
2 = B01011011
3 = B01001111
4 = B01100110
5 = B01101101
6 = B01111101
7 = B00000111
8 = B01111111
9 = B01101111
- = B01000000
```

## Button Functions

### Normal Weighing Mode

**ZERO**

* Tares the weighing machine.
* Sets the current weight to zero.

### Calibration Mode

**CAL**

Starts calibration.

The display initially shows:

```text
000000
```

**SHIFT**

Moves the selected digit from right to left.

**UP**

Increases the selected digit by one.

The selected digit blinks to show which digit is being edited.

**ZERO**

Confirms the entered calibration weight and continues the calibration process.

## Calibration Process

1. Press **CAL**.
2. The display shows:

```text
000000
```

3. Use **SHIFT** to select a digit.
4. Use **UP** to increase the selected digit.
5. Enter the known calibration weight in grams.
6. Press **ZERO** to confirm.
7. Remove all weight when requested.
8. The system records the load-cell zero value.
9. Place the known calibration weight on the platform.
10. Press **ZERO**.
11. The system calculates a new calibration factor.
12. The new factor is saved to EEPROM.
13. The machine returns to normal weighing mode.

Maximum calibration weight:

```text
50000 g
```

## EEPROM

The calibration factor is stored in the Arduino Nano's EEPROM.

EEPROM addresses used:

```cpp
const int EEPROM_MAGIC_ADDR = 0;
const int EEPROM_FACTOR_ADDR = 4;
```

A magic value is used to determine whether valid calibration data is stored.

```cpp
const uint16_t EEPROM_MAGIC = 0x5A3C;
```

When the machine starts:

1. EEPROM is checked.
2. If valid calibration data exists, the saved calibration factor is loaded.
3. Otherwise, the default factor is used.

Default calibration factor:

```text
420.0
```

After successful calibration, the new factor is automatically saved.

## Negative Weight Display

Negative values are displayed from the right side in the same way as positive values.

The minus sign is displayed on the **leftmost digit (Digit 6)**.

Examples:

```text
-5

- _ _ _ _ 5
```

```text
-50

- _ _ _ 5 0
```

```text
-500

- _ _ 5 0 0
```

```text
-50000

- 5 0 0 0 0
```

This keeps the weight value right-aligned.

## Software Requirements

Install:

* Arduino IDE
* HX711 Arduino library
* EEPROM Library

The project is intended for an **Arduino Nano / ATmega328P**.

## Uploading the Project

Place all files in the same Arduino sketch folder:

```text
WeighingMachine/
```

Open:

```text
WeighingMachine.ino
```

in Arduino IDE.

Select:

```text
Board: Arduino Nano
Processor: ATmega328P
```

Select the correct COM port and upload the sketch.

## Code Organization

### `WeighingMachine.ino`

Contains:

* Arduino setup
* Main loop
* HX711 object
* Normal weighing logic
* Weight reading
* Main state handling

### `Display.cpp`

Contains:

* 7-segment pin configuration
* Number patterns
* Minus pattern
* Display multiplexing
* Number and weight display functions
* Calibration digit blinking

### `Buttons.cpp`

Contains:

* Button pin configuration
* Button state handling
* Debouncing
* Button press detection

### `Calibration.cpp`

Contains:

* Calibration states
* Calibration weight entry
* Digit editing
* Zero/load measurements
* Calibration-factor calculation

### `EEPROMData.cpp`

Contains:

* EEPROM addresses
* EEPROM magic value
* Saving calibration factor
* Loading calibration factor

## Calibration Formula

The calibration factor is calculated using the difference between the unloaded and loaded HX711 readings.

```text
Raw Difference = Raw Load - Raw Zero
```

The known calibration weight is converted from grams to kilograms.

```text
Known Weight (kg) = Calibration Weight (g) / 1000
```

The new calibration factor is then:

```text
Calibration Factor =
Raw Difference / Known Weight (kg)
```

The calculated factor is stored in EEPROM.

## Safety / Limitations

* Do not exceed the rated **50 kg** load-cell capacity.
* The mechanical platform should be properly supported.
* Keep wiring secure and protected from mechanical movement.
* Calibration should be performed using a known and stable reference weight.
* The accuracy of the final machine depends on the load cell, mechanical structure, wiring, HX711, calibration, and environmental conditions.

## Future Improvements

Possible future improvements include:

* Improved filtering and averaging
* More accurate zero tracking
* Overload indication
* Battery operation
* Low-battery indication
* Buzzer/alarm
* Larger display
* Multiple calibration profiles
* Automatic calibration assistance
* Improved button interface
* Enclosure and commercial weighing-machine design

## License

This project is currently under open development. No formal license has been applied at this time.

The source code is provided for educational, research, development, and experimental purposes. Users may use, modify, and adapt the code for their own projects.

## Disclaimer

This project is provided for educational, research, and development purposes only.

Anyone who uses, modifies, copies, or distributes this source code is solely responsible for their own use of the project and must ensure that their activities comply with applicable laws and regulations.
