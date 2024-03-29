#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
//Board = Arduino Micro
#define __AVR_ATmega32U4__
#define ARDUINO 105
#define __AVR__
#define F_CPU 16000000L
#define __cplusplus
#define __attribute__(x)
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__
#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define prog_void
#define PGM_VOID_P int
#define NOINLINE __attribute__((noinline))

typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}

//
//
void readySound();
void fullPowerSound();
void startSound();
void calibrationSound();
void blink(int count);
void waitForButton();
void calibrate();
void race();
void rampThrottle(float target, long duration);
void setThrottle(float newThrottle);
void config();
void setPwmFrequency(int pin, int divisor);

#include "C:\alanc\arduino-1.0.5\hardware\arduino\variants\micro\pins_arduino.h" 
#include "C:\alanc\arduino-1.0.5\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Users\alco\Documents\Arduino\TetherCar\TetherCar.ino"
#endif
