#include <Arduino.h>
#include "HX711.h"

// =====================================================
// CORE HARDWARE
// =====================================================
#define DOUT 13
#define SCK  12

HX711 scale;

// Main weighing values
float calibration_factor = 420.0;
long grams = 0;
unsigned long lastWeightRead = 0;

// =====================================================
// MODULES
// =====================================================
#include "Display.h"
#include "Buttons.h"
#include "Calibration.h"
#include "EEPROMData.h"

// =====================================================
// NORMAL MODE
// =====================================================
void handleNormalMode() {
  // Read weight every 100 ms
  if (millis() - lastWeightRead >= 100) {
    lastWeightRead = millis();

    if (scale.is_ready()) {
      float weightKg = scale.get_units(2);
      float weightGrams = weightKg * 1000.0;

      // 50 kg maximum in either direction
      if (weightGrams > 50000) weightGrams = 50000;
      if (weightGrams < -50000) weightGrams = -50000;

      grams = round(weightGrams);

      Serial.print("Weight: ");
      Serial.print(grams);
      Serial.println(" g");
    }
  }

  // CAL button
  bool calNow = readCalButton();
  if (calNow && !oldCal) {
    startCalibration();
  }
  oldCal = calNow;

  // ZERO / TARE button
  bool zeroNow = readZeroButton();
  if (zeroNow && !oldZero) {
    scale.tare(20);
    grams = 0;
    Serial.println("ZERO / TARE");
    delay(100);
  }
  oldZero = zeroNow;

  displayWeight(grams);
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(9600);

  initDisplay();
  initButtons();

  scale.begin(DOUT, SCK);
  delay(1000);

  // Load saved calibration factor before taring.
  loadCalibrationFactor();

  Serial.println("Remove all weight...");
  delay(3000);

  scale.set_scale(calibration_factor);
  scale.tare(20);

  Serial.println("Scale ready");
  Serial.print("Calibration factor = ");
  Serial.println(calibration_factor, 4);

  grams = 0;
}

// =====================================================
// MAIN LOOP
// =====================================================
void loop() {
  if (calState == NORMAL) {
    handleNormalMode();
  }
  else if (calState == CAL_EDIT) {
    handleCalibrationEdit();
  }
  else if (calState == CAL_WAIT_WEIGHT) {
    handleCalibrationWeight();
  }
}
