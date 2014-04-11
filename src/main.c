/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f0xx_conf.h"
#include "stm32f0xx.h"

#include "../hd44780/hd44780.h"
#include "../slow_timer/slow_timer.h"
#include "../common/common_funcs.h"

#include "../inc/buttons.h"
#include "../menu/menu.h"
#include "../flash/flash.h"
//#include "../globs.h"

void Init(void);

uint8_t button;
char my_data[3];


int main(void)
{
    Init();
    InitMenu();
    ProcessMenu(BTN3);

  while(1)
  {
    button = read_btn();
    if(button){
        ProcessMenu(button);
        //clean_btn();
    }

  }
}

void Init(void){
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    GPIOC->MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0); //Pins 8,9 as output
    GPIOC->MODER &= ~(GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1);
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9); //Output push-pull
    GPIOC->OSPEEDR |= (GPIO_OSPEEDR_OSPEEDR8 | GPIO_OSPEEDR_OSPEEDR9); //High speed

    buttons_init();

    lcd_init();

    Init_Slow_Timer();

    Slow_Timer_Add(tm_Repeat, 10, check_btn);
}




