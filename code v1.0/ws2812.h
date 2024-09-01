#pragma once
#include "hardware/pio.h"
#include "inttypes.h"

#define WS2812_SM 1
#define WS2812_PIO ((pio_hw_t *)PIO1_BASE)
#define WS2812_PIN 23 // 23-black pico 16-zero pico

void ws2812_set_rgb(uint8_t red, uint8_t green, uint8_t blue);
void ws2812_init();
void ws2812_reset();
