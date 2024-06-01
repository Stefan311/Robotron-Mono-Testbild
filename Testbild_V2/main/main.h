#include <stdio.h>
#include <stdint.h>
#include "driver/dedic_gpio.h"
#include "hal/dedic_gpio_cpu_ll.h"
#include "driver/gpio.h"
#include <xtensa/core-macros.h>
#include "freertos/FreeRTOS.h"

#define PIN_NUM_OUT1 45     // BSYN
#define PIN_NUM_OUT2 47     // VIDEO1
#define PIN_NUM_OUT3 48     // VIDEO2

#define PIN_NUM_BUTTON1 38  // Taster A7100-Modus
#define PIN_NUM_BUTTON2 39  // Taster PC1715-Modus
#define PIN_NUM_BUTTON3 40  // Taster EC1845-Modus

extern void A7100();
extern void PC1715();
extern void EC1834();
extern void wait_until(uint32_t time);

#define NOP asm volatile ("nop");
#define SET_GPIO(v) asm volatile ("EE.SET_BIT_GPIO_OUT " #v);
#define CLR_GPIO(v) asm volatile ("EE.CLR_BIT_GPIO_OUT " #v);

