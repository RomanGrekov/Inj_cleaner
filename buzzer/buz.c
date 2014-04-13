#include "buz.h"
#include "../globs.h"

void InitBuz (void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    GPIOC->MODER |= GPIO_MODER_MODER12_0; //Pins 8,9 as output
    GPIOC->MODER &= ~GPIO_MODER_MODER12_1;
    GPIOC->OTYPER &= ~GPIO_OTYPER_OT_12; //Output push-pull
    GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR12; //High speed
}

void Bzz(uint8_t variant)
{
	uint32_t lenght, d1, d2;
	uint32_t i;

	switch (variant)
	{
	case 0:
		lenght = 80;
		d1 = 40;
		d2 = 6;
		break;
	case 1:
		lenght = 120;
		d1 = 15;
		d2 = 5;
		break;
	}

	for (i=0; i<lenght; i++)
	{
		GPIOC->ODR |= GPIO_Pin_12 ;
		for(uint32_t i=0; i<((F_CPU/1000000)*d1); i++);
		GPIOC->ODR &= ~GPIO_Pin_12;
		for(uint32_t i=0; i<((F_CPU/1000000)*d2); i++);
	}
}
