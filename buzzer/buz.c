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
    uint8_t x=5, y=2;
	uint32_t lenght, d[x][y];
	uint32_t i=0, ii=0, l=0;

	switch (variant)
	{
	case 0:
		lenght = 20;
        d[0][0]=50;d[0][1]=5;
        d[1][0]=20;d[1][1]=5;
        d[2][0]=30;d[2][1]=5;
        d[3][0]=50;d[3][1]=5;
        d[4][0]=50;d[4][1]=5;
		break;
	case 1:
		lenght = 100;
        d[0][0]=50;d[0][1]=5;
        d[1][0]=30;d[1][1]=5;
        d[2][0]=50;d[2][1]=5;
        d[3][0]=30;d[3][1]=5;
        d[4][0]=50;d[4][1]=5;
		break;
    case 2:
		lenght = 50;
        d[0][0]=10;d[0][1]=10;
        d[1][0]=0;d[1][1]=0;
        d[2][0]=0;d[2][1]=0;
        d[3][0]=0;d[3][1]=0;
        d[4][0]=0;d[4][1]=0;
		break;
	}


    for(i=0; i<x; i++){
        for (l=0; l<lenght; l++){
            GPIOC->ODR |= GPIO_Pin_12 ;
            for(uint32_t t=0; t<((F_CPU/1000000)*d[i][0]); t++);
            GPIOC->ODR &= ~GPIO_Pin_12;
            for(uint32_t t=0; t<((F_CPU/1000000)*d[i][1]); t++);

	    }
	}
}
