#include <stdio.h>
#include "hal/dedic_gpio_cpu_ll.h"
#include "driver/gpio.h"
#include "driver/dedic_gpio.h"
#include <xtensa/core-macros.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "sdkconfig.h"

#define PIN_NUM_OUT1 45
#define PIN_NUM_OUT2 47
#define PIN_NUM_OUT3 48
#define PIN_NUM_OUT4 18

#define PIN_NUM_BUTTON1 38
#define PIN_NUM_BUTTON2 39
#define PIN_NUM_BUTTON3 40
#define PIN_NUM_BUTTON4 41

extern const uint16_t A7100_Data[] asm("A7100_bin");
extern const uint16_t A7100_len asm("A7100_bin_length");
extern const uint16_t PC1715_Data[] asm("PC1715_bin");
extern const uint16_t PC1715_len asm("PC1715_bin_length");
extern const uint16_t EC1834_Data[] asm("EC1834_bin");
extern const uint16_t EC1834_len asm("EC1834_bin_length");

void IRAM_ATTR wait_until(uint32_t time)
{
    asm volatile (
        "   addi    a14, %0, -31    \n"
        "Delay:                     \n"
        "   rsr     a13, ccount     \n"
        "   blt     a13, a14, Delay \n"
        "   sub     a14, a13, a14   \n"
        "   extui   a14, a14, 0, 4  \n"
        "   movi    a13, NopTable   \n"
        "   addx2   a13, a14, a13   \n"
        "   jx      a13             \n"
        ".align	4                   \n"
        "NopTable:                  \n"
        "   nop.n                   \n"
        "   nop.n                   \n"
        "   nop.n                   \n"
        "   nop.n                   \n"
        "   nop.n                   \n"
        "   nop.n                   \n"
        "   nop.n                   \n"
        "   nop.n                   \n"
        "   nop.n                   \n"
        "   nop.n                   \n"
        "   nop.n                   \n"
        "   nop.n                   \n"
        "   nop.n                   \n"
        "   nop.n                   \n"
        "   nop.n                   \n"
        "   nop.n                   \n"
        ::"r" (time):"a13","a14" );
}


void IRAM_ATTR app_main(void)
{
    const int gpios[4] = {PIN_NUM_OUT1, PIN_NUM_OUT2, PIN_NUM_OUT3, PIN_NUM_OUT4}; 
    dedic_gpio_bundle_handle_t test_bundle = NULL;   
    dedic_gpio_bundle_config_t bundle_config = 
    {
        .gpio_array = gpios,
        .array_size = 4,
        .flags = 
        {
            .out_en = 1,
        },
    };
    dedic_gpio_new_bundle(&bundle_config, &test_bundle);
    
	gpio_config_t pincfg =
	{
		.pin_bit_mask = 1ULL<<PIN_NUM_BUTTON1 | 1ULL<<PIN_NUM_BUTTON2 | 1ULL<<PIN_NUM_BUTTON3 | 1ULL<<PIN_NUM_BUTTON4,
	    .mode = GPIO_MODE_INPUT,
	    .pull_up_en = true,
	    .pull_down_en = false,
	};
	ESP_ERROR_CHECK(gpio_config(&pincfg));

    portMUX_TYPE lock = portMUX_INITIALIZER_UNLOCKED;
    taskENTER_CRITICAL(&lock);

    uint16_t *start = heap_caps_malloc(0xffff, MALLOC_CAP_INTERNAL);
    memcpy(start,A7100_Data,A7100_len);
    uint16_t *ende = start + (A7100_len/2);
    uint16_t *data;

    while (1)
    {
        if (gpio_get_level(PIN_NUM_BUTTON1)==0)
        {
            memcpy(start,A7100_Data,A7100_len);
            ende = start + (A7100_len/2);
        }
        if (gpio_get_level(PIN_NUM_BUTTON2)==0)
        {
            memcpy(start,PC1715_Data,PC1715_len);
            ende = start + (PC1715_len/2);
        }
        if (gpio_get_level(PIN_NUM_BUTTON3)==0)
        {
            memcpy(start,EC1834_Data,EC1834_len);
            ende = start + (EC1834_len/2);
        }
        if (gpio_get_level(PIN_NUM_BUTTON4)==0)
        {
            
        }
        data = start;
        XTHAL_SET_CCOUNT(0);
        uint32_t time = 0;
        uint16_t value = 0;

        while (data<ende)
        {
            value = *data;
            time += (value>>4) & 0xfff;
            wait_until(time);
            dedic_gpio_cpu_ll_write_all(value);
            data++;
        }
    };

}