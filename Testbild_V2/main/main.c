#include "main.h"

// wartet auf das erreichen des angegebenen CPU-Zyklus
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
    // GPIO OUT-Pins CPU-Abkürzung einstellen
    const int gpios[4] = {PIN_NUM_OUT1, PIN_NUM_OUT2, PIN_NUM_OUT3}; 
    dedic_gpio_bundle_handle_t test_bundle = NULL;   
    dedic_gpio_bundle_config_t bundle_config = 
    {
        .gpio_array = gpios,
        .array_size = 3,
        .flags = 
        {
            .out_en = 1,
        },
    };
    dedic_gpio_new_bundle(&bundle_config, &test_bundle);
    gpio_set_drive_capability(PIN_NUM_OUT1, GPIO_DRIVE_CAP_3);
    gpio_set_drive_capability(PIN_NUM_OUT2, GPIO_DRIVE_CAP_3);
    gpio_set_drive_capability(PIN_NUM_OUT3, GPIO_DRIVE_CAP_3);
    
    // Modus-Tasten IN-Pins einstellen
	gpio_config_t pincfg =
	{
		.pin_bit_mask = 1ULL<<PIN_NUM_BUTTON1 | 1ULL<<PIN_NUM_BUTTON2 | 1ULL<<PIN_NUM_BUTTON3,
	    .mode = GPIO_MODE_INPUT,
	    .pull_up_en = true,
	    .pull_down_en = false,
	};
	ESP_ERROR_CHECK(gpio_config(&pincfg));

    // Bitte nicht stören!
    portMUX_TYPE lock = portMUX_INITIALIZER_UNLOCKED;
    taskENTER_CRITICAL(&lock);

    int mode = 0;
    while (1)
    {
        // Modus-Tasten abfragen
        if (gpio_get_level(PIN_NUM_BUTTON1)==0)
        {
            mode = 0;
        }
        if (gpio_get_level(PIN_NUM_BUTTON2)==0)
        {
            mode = 1;
        }
        if (gpio_get_level(PIN_NUM_BUTTON3)==0)
        {
            mode = 2;
        }

        // Sampler starten
        switch (mode)
        {
            case 0: 
                A7100();
                break;
            case 1: 
                PC1715();
                break;
            case 2: 
                EC1834();
                break;
        }
    };
}