#include "Display.h"
#include "Calibration.h"

// =====================================================
// DISPLAY HARDWARE
// =====================================================

const byte segPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
const byte digitPins[6] = {10, 11, A0, A1, A2, A3};

const byte numbers[10] = {
  B00111111, // 0
  B00000110, // 1
  B01011011, // 2
  B01001111, // 3
  B01100110, // 4
  B01101101, // 5
  B01111101, // 6
  B00000111, // 7
  B01111111, // 8
  B01101111  // 9
};

const byte MINUS_SIGN = B01000000;

// =====================================================
// INITIALIZE DISPLAY
// =====================================================
void initDisplay() {
  for (byte i = 0; i < 8; i++) {
    pinMode(segPins[i], OUTPUT);
    digitalWrite(segPins[i], LOW);
  }

  for (byte i = 0; i < 6; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH);
  }
}

// =====================================================
// CLEAR DISPLAY
// =====================================================
void clearDisplay() {
  for (byte i = 0; i < 6; i++) {
    digitalWrite(digitPins[i], HIGH);
  }

  for (byte i = 0; i < 8; i++) {
    digitalWrite(segPins[i], LOW);
  }
}

// =====================================================
// CALIBRATION DISPLAY
// =====================================================
void displayCalibration(unsigned long value) {
  byte digit[6];

  digit[0] = value % 10;
  digit[1] = (value / 10) % 10;
  digit[2] = (value / 100) % 10;
  digit[3] = (value / 1000) % 10;
  digit[4] = (value / 10000) % 10;
  digit[5] = (value / 100000) % 10;

  for (byte d = 5; d < 6; d--) {
    for (byte i = 0; i < 6; i++) {
      digitalWrite(digitPins[i], HIGH);
    }

    byte pattern = numbers[digit[d]];

    for (byte s = 0; s < 8; s++) {
      digitalWrite(segPins[s], bitRead(pattern, s));
    }

    digitalWrite(digitPins[d], LOW);
    delayMicroseconds(1000);
    digitalWrite(digitPins[d], HIGH);

    if (d == 0) break;
  }
}

// =====================================================
// BLINK CALIBRATION DISPLAY
// =====================================================
void blinkCalibrationDisplay(byte times) {
  for (byte i = 0; i < times; i++) {
    clearDisplay();
    delay(80);

    displayCalibration(calibrationGrams);
    delay(80);
  }
}

// =====================================================
// BLINK SELECTED CALIBRATION DIGIT
// =====================================================
void blinkSelectedDigit() {
  byte digit[6];

  digit[0] = calibrationGrams % 10;
  digit[1] = (calibrationGrams / 10) % 10;
  digit[2] = (calibrationGrams / 100) % 10;
  digit[3] = (calibrationGrams / 1000) % 10;
  digit[4] = (calibrationGrams / 10000) % 10;
  digit[5] = (calibrationGrams / 100000) % 10;

  // OFF
  for (byte i = 0; i < 6; i++) {
    digitalWrite(digitPins[i], HIGH);
  }
  delay(80);

  // ON selected digit
  byte pattern = numbers[digit[selectedDigit]];

  for (byte s = 0; s < 8; s++) {
    digitalWrite(segPins[s], bitRead(pattern, s));
  }

  digitalWrite(digitPins[selectedDigit], LOW);
  delay(80);
  digitalWrite(digitPins[selectedDigit], HIGH);
}

// =====================================================
// NORMAL WEIGHT DISPLAY
// =====================================================
// Positive values are right-aligned across all 6 digits.
// Negative values use Digit 6 for minus and Digits 1-5
// for the magnitude, also generated from right to left.
// =====================================================
void displayWeight(long value) {
  byte digit[6];

  // 255 = blank
  for (byte i = 0; i < 6; i++) {
    digit[i] = 255;
  }

  if (value >= 0) {
    unsigned long v = value;

    for (byte i = 0; i < 6; i++) {
      if (v > 0 || i == 0) {
        digit[i] = v % 10;
        v /= 10;
      }
    }
  }
  else {
    unsigned long v = (unsigned long)(-value);

    // Digits 1-5 hold the negative magnitude.
    for (byte i = 0; i < 5; i++) {
      if (v > 0 || i == 0) {
        digit[i] = v % 10;
        v /= 10;
      }
    }
  }

  // Multiplex all six digits.
  for (byte i = 0; i < 6; i++) {
    // All digits OFF
    for (byte j = 0; j < 6; j++) {
      digitalWrite(digitPins[j], HIGH);
    }

    // All segments OFF
    for (byte s = 0; s < 8; s++) {
      digitalWrite(segPins[s], LOW);
    }

    // Digit 6 = minus sign for negative weight
    if (value < 0 && i == 5) {
      for (byte s = 0; s < 8; s++) {
        digitalWrite(segPins[s], bitRead(MINUS_SIGN, s));
      }
    }
    // Blank unused digit
    else if (digit[i] == 255) {
      // Already OFF
    }
    // Number
    else {
      byte pattern = numbers[digit[i]];

      for (byte s = 0; s < 8; s++) {
        digitalWrite(segPins[s], bitRead(pattern, s));
      }
    }

    digitalWrite(digitPins[i], LOW);
    delayMicroseconds(1000);
  }
}
