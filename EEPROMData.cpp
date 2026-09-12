#include "EEPROMData.h"
#include <EEPROM.h>
#include <math.h>

// Defined in WeighingMachine.ino.
extern float calibration_factor;

const int EEPROM_MAGIC_ADDR = 0;
const int EEPROM_FACTOR_ADDR = 4;
const uint16_t EEPROM_MAGIC = 0x5A3C;

// =====================================================
// LOAD CALIBRATION FACTOR
// =====================================================
void loadCalibrationFactor() {
  uint16_t magic = 0;
  EEPROM.get(EEPROM_MAGIC_ADDR, magic);

  if (magic == EEPROM_MAGIC) {
    float savedFactor = 0.0;
    EEPROM.get(EEPROM_FACTOR_ADDR, savedFactor);

    if (isfinite(savedFactor) && savedFactor > 0.0) {
      calibration_factor = savedFactor;

      Serial.print("EEPROM calibration factor = ");
      Serial.println(calibration_factor, 4);
    }
    else {
      Serial.println("EEPROM factor invalid. Using default 420.0");
      calibration_factor = 420.0;
    }
  }
  else {
    Serial.println("No saved calibration found. Using default 420.0");
    calibration_factor = 420.0;
  }
}

// =====================================================
// SAVE CALIBRATION FACTOR
// =====================================================
void saveCalibrationFactor() {
  EEPROM.put(EEPROM_FACTOR_ADDR, calibration_factor);
  EEPROM.put(EEPROM_MAGIC_ADDR, EEPROM_MAGIC);

  Serial.println("Calibration factor saved to EEPROM.");
}
