#include "test1.h"

void UpdateLcd(void);
void EncoderScan_1(void);
void EncoderScan_2(void);
void LED8(uint8_t state);

uint16_t enc1=0, old_enc1;
uint16_t enc2=0, old_enc2;
uint16_t step1=10;
uint16_t step2=10;

uint8_t button;

uint8_t pwm_state=0;

void test1(void){
    lcd_clrscr();
    lcd_prints("Test 1 selected");

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    GPIOC->MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0); //Pins 8,9 as output
    GPIOC->MODER &= ~(GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1);
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9); //Output push-pull
    GPIOC->OSPEEDR |= (GPIO_OSPEEDR_OSPEEDR8 | GPIO_OSPEEDR_OSPEEDR9); //High speed

    PWM_init();

    EncoderInit();

    Slow_Timer_Add(tm_Repeat, 100, UpdateLcd);
    Slow_Timer_Add(tm_Repeat, 1, EncoderScan_1);
    Slow_Timer_Add(tm_Repeat, 1, EncoderScan_2);

    while(1)
    {
        button = read_btn();
        if(button){
            if(button == BTN4) pwm_state = ~pwm_state;

            if(pwm_state)PWM_start();
            else PWM_stop();
            LED8(pwm_state);
        }

    }
}

void UpdateLcd(void){
    char my_char[8]={0};

    if(old_enc1 != enc1 || old_enc2 != enc2){
        lcd_clrscr();
        lcd_goto(2,0);
        lcd_prints("H:");
        itoa_fix(enc1, 10, 5, my_char);
        lcd_prints(my_char);
        lcd_prints("  ");
        lcd_prints("L:");
        itoa_fix(enc2, 10, 5, my_char);
        lcd_prints(my_char);
        old_enc1 = enc1;
        old_enc2 = enc2;

        PWM_set(enc1, enc2);
        //PWM_set(10, 50);

    }
}

void EncoderScan_1(void){
    EncoderScan1(&enc1, step1, 0, 24000);
}

void EncoderScan_2(void){
    EncoderScan2(&enc2, step2, 0, 24000);
}

void LED8(uint8_t state){
    if(state) GPIOC->ODR |= GPIO_Pin_8 ;
    if(!state) GPIOC->ODR &= ~GPIO_Pin_8;
}
