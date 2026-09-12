#include "Buttons.h"

bool oldCal = false;
bool oldUp = false;
bool oldShift = false;
bool oldZero = false;

void initButtons() {
  // A6 and A7 use external 10k pull-ups.
  pinMode(SHIFT_BUTTON, INPUT_PULLUP);
  pinMode(ZERO_BUTTON, INPUT_PULLUP);
}

bool readCalButton() {
  return analogRead(CAL_BUTTON) < 100;
}

bool readUpButton() {
  return analogRead(UP_BUTTON) < 100;
}

bool readShiftButton() {
  return digitalRead(SHIFT_BUTTON) == LOW;
}

bool readZeroButton() {
  return digitalRead(ZERO_BUTTON) == LOW;
}
