#pragma once




#include "inttypes.h"
#include "hardware/gpio.h"
#include <hardware/clocks.h>

#include <pico/time.h>

#include "hardware/clocks.h"
#include <pico/stdlib.h>
#include <hardware/vreg.h>


#include <hardware/structs/vreg_and_chip_reset.h>

#include <pico/bootrom.h>
#include <pico/bootrom.h>

#include "logger.h"
#include "scancode_zx.h"
#include "ws2812.h"


// частота RP2040
#define CPU_MHZ 133//240//set_sys_clock_khz(300000, true); 
#define VOLTAGE VREG_VOLTAGE_1_15 //VREG_VOLTAGE_1_30 //	vreg_set_voltage(VOLTAGE); 

// пины пико для управления 8816

#define AX0 2
#define AX1 3
#define AX2 4
#define AX3 5
#define AY0 6
#define AY1 7
#define AY2 8
#define RSTMT 9
#define CSMT  10
#define DATMT 11
#define STBMT 12

#define LEDPIN 25 //  Black pico
#define LEDPIN 25 //  zero pico


// макросы
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bit(b) (1UL << (b))
// макросы end