#pragma once

#include <avr/io.h>

// cpu
#define F_CPU 16000000UL

// gears
#define TABLE_STEPPER_STEPS     200
#define TABLE_STEPPER_MICRO     16
#define TABLE_REDUCER_BIG       25
#define TABLE_REDUCER_SMALL     1
#define TABLE_DIR               1

#define MIRROR_STEPPER_STEPS    200
#define MIRROR_STEPPER_MICRO    16
#define MIRROR_REDUCER_BIG      58
#define MIRROR_REDUCER_SMALL    15
#define MIRROR_DIR              1

#define X_STEPPER_STEPS         200
#define X_STEPPER_MICRO         16
#define X_GEAR_NTEETH           15
#define X_GEAR_TOOTH            2.0

// pins
#define MIRROR_PORT PORTD
#define MIRROR_DDR  DDRD
#define MIRROR_STEP_PIN 3
#define MIRROR_DIR_PIN 6

#define TABLE_PORT PORTD
#define TABLE_DDR  DDRD
#define TABLE_STEP_PIN 4
#define TABLE_DIR_PIN  7

#define X_PORT PORTD
#define X_DDR  DDRD
#define X_STEP_PIN 2
#define X_DIR_PIN  5

#define EN_PORT PORTB
#define EN_DDR  DDRB
#define EN_PIN 0

#define SPRAY_RUN_PORT PORTB
#define SPRAY_RUN_DDR  DDRB
#define SPRAY_RUN_PIN 4

#define SPRAY_REFILL_PORT PORTB
#define SPRAY_REFILL_DDR DDRB
#define SPRAY_REFILL_PIN 5

#define SPRAY_ENDSTOP_PORT PORTC
#define SPRAY_ENDSTOP_PIN PINC
#define SPRAY_ENDSTOP_DDR DDRC
#define SPRAY_ENDSTOP_BIT 3

// Serial
#define BAUD 9600UL
