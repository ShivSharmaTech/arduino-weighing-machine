#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

// =====================================================
// BUTTON PINS
// =====================================================
#define CAL_BUTTON   A6
#define UP_BUTTON    A7
#define SHIFT_BUTTON A4
#define ZERO_BUTTON  A5

// Button edge memory
extern bool oldCal;
extern bool oldUp;
extern bool oldShift;
extern bool oldZero;

// =====================================================
// BUTTON FUNCTIONS
// =====================================================
void initButtons();
bool readCalButton();
bool readUpButton();
bool readShiftButton();
bool readZeroButton();

#endif
