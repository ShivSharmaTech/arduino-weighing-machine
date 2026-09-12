#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

// =====================================================
// DISPLAY HARDWARE
// =====================================================

// a,b,c,d,e,f,g,dp
extern const byte segPins[8];

// Digit 1 = RIGHTMOST
// Digit 6 = LEFTMOST
extern const byte digitPins[6];

// Common cathode: HIGH = segment ON
extern const byte numbers[10];
extern const byte MINUS_SIGN;

// =====================================================
// DISPLAY FUNCTIONS
// =====================================================
void initDisplay();
void clearDisplay();
void displayCalibration(unsigned long value);
void blinkCalibrationDisplay(byte times);
void blinkSelectedDigit();
void displayWeight(long value);

#endif
