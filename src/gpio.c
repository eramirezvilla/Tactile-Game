// COMP-GENG 421 - Edgar Ramirez-Villa
//
// GPIO module
//

#include "main.h"
#include "gpio.h"


void GpioInit(void)
{
	// Enable the port clocks
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);

	// GPIO PA5 -> Green LED (active high)
	// Configure PA5 as an output, green LED off
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);

	// PA9 -> RGB blue LED (active low)
	// Configure PA9 as an output, RGB blue LED off
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_9);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT);

	// PB4 -> RGB red LED (active low)
	// Configure PB4 as an output, RGB red LED off
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_4, LL_GPIO_MODE_OUTPUT);

	// PC4 -> debug output (active high)
	// Configure PC4 as an output, debug signal low
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_4);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_4, LL_GPIO_MODE_OUTPUT);

	// PC7 -> RGB green LED (active low)
	// Configure PC7 as an output, RGB green LED off
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_7);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
}
