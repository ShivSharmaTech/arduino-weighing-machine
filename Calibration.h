#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <Arduino.h>

// =====================================================
// CALIBRATION STATES
// =====================================================
enum CalState {
  NORMAL,
  CAL_EDIT,
  CAL_WAIT_WEIGHT
};

extern CalState calState;
extern unsigned long calibrationGrams;
extern byte selectedDigit;
extern long rawZero;

// =====================================================
// CALIBRATION FUNCTIONS
// =====================================================
void startCalibration();
void increaseSelectedDigit();
void handleCalibrationEdit();
void handleCalibrationWeight();

#endif
