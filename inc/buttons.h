#ifndef BUTTONS_H_INCLUDED
#define BUTTONS_H_INCLUDED
#include "stm32f0xx.h"

#define treshhold 5

enum{
    ENC1_BTN=1,
    ENC2_BTN,
    BTN1,
    BTN2,
    BTN3,
    BTN4,
};
void buttons_init(void);
void check_btn(void);
uint8_t read_btn(void);

#endif /* BUTTONS_H_INCLUDED */
