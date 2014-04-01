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


void Init(void);


uint8_t button;

int main(void)
{
    Init();
    InitMenu();
    ProcessMenu(9);
  while(1)
  {

    button = read_btn();
    if(button){
        ProcessMenu(button);
    }
  }
}

void Init(void){
    buttons_init();

    lcd_init();

    Init_Slow_Timer();

    Slow_Timer_Add(tm_Repeat, 20, check_btn);

}





