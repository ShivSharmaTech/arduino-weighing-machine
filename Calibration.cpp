#include "Calibration.h"
#include "HX711.h"
#include "Display.h"
#include "Buttons.h"
#include "EEPROMData.h"

// These are defined in WeighingMachine.ino
extern HX711 scale;
extern float calibration_factor;
extern long grams;

CalState calState = NORMAL;
unsigned long calibrationGrams = 0;
byte selectedDigit = 0;
long rawZero = 0;

// =====================================================
// START CALIBRATION
// =====================================================
void startCalibration() {
  // Always start calibration from 000000.
  calibrationGrams = 0;
  selectedDigit = 0;
  calState = CAL_EDIT;

  Serial.println();
  Serial.println("========================");
  Serial.println("CALIBRATION MODE");
  Serial.println("Display = 000000");
  Serial.println("SHIFT = select digit");
  Serial.println("UP = increase digit");
  Serial.println("ZERO = confirm");
  Serial.println("========================");

  blinkCalibrationDisplay(2);
}

// =====================================================
// INCREASE SELECTED DIGIT
// =====================================================
void increaseSelectedDigit() {
  byte digits[6];

  digits[0] = calibrationGrams % 10;
  digits[1] = (calibrationGrams / 10) % 10;
  digits[2] = (calibrationGrams / 100) % 10;
  digits[3] = (calibrationGrams / 1000) % 10;
  digits[4] = (calibrationGrams / 10000) % 10;
  digits[5] = (calibrationGrams / 100000) % 10;

  digits[selectedDigit]++;

  if (digits[selectedDigit] > 9) {
    digits[selectedDigit] = 0;
  }

  calibrationGrams =
      digits[0]
    + digits[1] * 10UL
    + digits[2] * 100UL
    + digits[3] * 1000UL
    + digits[4] * 10000UL
    + digits[5] * 100000UL;

  // Maximum calibration weight = 50000 g.
  if (calibrationGrams > 50000) {
    calibrationGrams = 0;
  }

  Serial.print("Calibration value = ");
  Serial.println(calibrationGrams);
}

// =====================================================
// CALIBRATION EDIT MODE
// =====================================================
void handleCalibrationEdit() {
  // UP BUTTON
  bool upNow = readUpButton();

  if (upNow && !oldUp) {
    increaseSelectedDigit();
    blinkCalibrationDisplay(1);
    delay(100);
  }
  oldUp = upNow;

  // SHIFT BUTTON
  bool shiftNow = readShiftButton();

  if (shiftNow && !oldShift) {
    // RIGHT TO LEFT
    selectedDigit++;

    if (selectedDigit > 5) {
      selectedDigit = 0;
    }

    Serial.print("Selected digit = ");
    Serial.println(selectedDigit);

    // Blink only selected digit.
    blinkSelectedDigit();
    delay(100);
  }
  oldShift = shiftNow;

  // ZERO = CONFIRM
  bool zeroNow = readZeroButton();

  if (zeroNow && !oldZero) {
    if (calibrationGrams == 0) {
      Serial.println("ERROR: Weight cannot be 0");
    }
    else if (calibrationGrams > 50000) {
      Serial.println("ERROR: Maximum is 50000 g");
    }
    else {
      Serial.println();
      Serial.println("========================");
      Serial.print("Known weight = ");
      Serial.print(calibrationGrams);
      Serial.println(" g");

      Serial.println("Taring empty platform...");
      scale.tare(10);

      // Raw empty reading.
      rawZero = scale.read_average(10);

      Serial.print("Raw ZERO = ");
      Serial.println(rawZero);

      Serial.println();
      Serial.println("PUT THE KNOWN WEIGHT");
      Serial.print("Put exactly ");
      Serial.print(calibrationGrams);
      Serial.println(" g");
      Serial.println("Then press ZERO.");
      Serial.println("========================");

      calState = CAL_WAIT_WEIGHT;
    }

    delay(100);
  }
  oldZero = zeroNow;

  displayCalibration(calibrationGrams);
}

// =====================================================
// WAIT FOR KNOWN WEIGHT
// =====================================================
void handleCalibrationWeight() {
  // Keep showing entered weight.
  displayCalibration(calibrationGrams);

  // ZERO = READ LOADED WEIGHT
  bool zeroNow = readZeroButton();

  if (zeroNow && !oldZero) {
    Serial.println();
    Serial.println("Reading loaded weight...");
    Serial.println("Please wait...");

    long rawLoad = scale.read_average(10);

    Serial.print("Raw LOAD = ");
    Serial.println(rawLoad);

    long rawDifference = rawLoad - rawZero;

    Serial.print("Raw difference = ");
    Serial.println(rawDifference);

    float knownWeightKg = calibrationGrams / 1000.0;
    float newFactor = (float)rawDifference / knownWeightKg;

    Serial.print("NEW CALIBRATION FACTOR = ");
    Serial.println(newFactor, 4);

    if (newFactor > 0) {
      calibration_factor = newFactor;
      scale.set_scale(calibration_factor);

      // Save the new factor to EEPROM.
      saveCalibrationFactor();

      Serial.println();
      Serial.println("========================");
      Serial.println("CALIBRATION SUCCESS");
      Serial.print("New factor = ");
      Serial.println(calibration_factor, 4);
      Serial.println("========================");

      delay(500);

      // Test current weight.
      float testKg = scale.get_units(5);

      Serial.print("TEST WEIGHT = ");
      Serial.print(testKg, 3);
      Serial.println(" kg");

      float testGrams = testKg * 1000.0;

      if (testGrams > 50000) testGrams = 50000;
      if (testGrams < -50000) testGrams = -50000;

      grams = round(testGrams);

      Serial.print("TEST DISPLAY = ");
      Serial.print(grams);
      Serial.println(" g");

      Serial.println();
      Serial.println("Calibration finished.");
      Serial.println("Remove weight.");

      calState = NORMAL;
    }
    else {
      Serial.println();
      Serial.println("CALIBRATION ERROR");
      Serial.println("Raw difference invalid.");
      Serial.println("Calibration NOT changed.");

      calState = NORMAL;
    }

    delay(100);
  }

  oldZero = zeroNow;
}
