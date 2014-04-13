#include "buttons.h"

volatile uint8_t btn=0;

void buttons_init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
     /* GPIOB Clocks enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    //0 - Enc1. 1 - Enc2. 2 - Btn1. 3 - Btn2. 4 - Btn3. 5 - Btn4.
    GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    btn=0;
}

void check_btn(void){
    static uint16_t  code=0, old_code=0;
    static uint16_t press_cnt=0;

    code = ~(GPIOB->IDR | 0b1111111111000000);

    if(code == old_code && code != 0) press_cnt++;

    if(press_cnt >= treshhold && code == 0){

        switch(old_code){
        case 1:
            btn = ENC1_BTN;
            break;
        case 2:
            btn = ENC2_BTN;
            break;
        case 4:
            btn = BTN1;
            break;
        case 8:
            btn = BTN2;
            break;
        case 16:
            btn = BTN3;
            break;
        case 32:
            btn = BTN4;
            break;
        }
        press_cnt=0;
    }
    old_code = code;
}

uint8_t read_btn(void){
    uint8_t btn_old;
    btn_old=btn;
    btn=0;
    return btn_old;
}

void clean_btn(void){
    btn=0;
}


