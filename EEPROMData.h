#ifndef EEPROM_DATA_H
#define EEPROM_DATA_H

#include <Arduino.h>

// =====================================================
// EEPROM DATA
// =====================================================

// Saved calibration factor survives power OFF/ON.
extern const int EEPROM_MAGIC_ADDR;
extern const int EEPROM_FACTOR_ADDR;
extern const uint16_t EEPROM_MAGIC;

// =====================================================
// EEPROM FUNCTIONS
// =====================================================
void loadCalibrationFactor();
void saveCalibrationFactor();

#endif
