#include "../inc/encoders.h"

#define __check_bit(N,I) GPIO_##I##_##N
#define _check_bit(N,I) __check_bit(N,I)
#define ENC1_CHECK_1 _check_bit(ENC1_PIN1, INP_REG)
#define ENC1_CHECK_2 _check_bit(ENC1_PIN2, INP_REG)
#define ENC2_CHECK_1 _check_bit(ENC2_PIN1, INP_REG)
#define ENC2_CHECK_2 _check_bit(ENC2_PIN2, INP_REG)

#define __moder_bit(N) GPIO_MODER_MODER##N
#define _moder_bit(N) __moder_bit(N)
#define ENC1_MODER_1 _moder_bit(ENC1_PIN1)
#define ENC1_MODER_2 _moder_bit(ENC1_PIN2)
#define ENC2_MODER_1 _moder_bit(ENC2_PIN1)
#define ENC2_MODER_2 _moder_bit(ENC2_PIN2)

#define __pupdr_bit0(N) GPIO_PUPDR_PUPDR##N##_0
#define _pupdr_bit0(N) __pupdr_bit0(N)
#define ENC1_PUPDR_1_0 _pupdr_bit0(ENC1_PIN1)
#define ENC1_PUPDR_2_0 _pupdr_bit0(ENC1_PIN2)
#define ENC2_PUPDR_1_0 _pupdr_bit0(ENC2_PIN1)
#define ENC2_PUPDR_2_0 _pupdr_bit0(ENC2_PIN2)

#define __pupdr_bit1(N) GPIO_PUPDR_PUPDR##N##_1
#define _pupdr_bit1(N) __pupdr_bit1(N)
#define ENC1_PUPDR_1_1 _pupdr_bit1(ENC1_PIN1)
#define ENC1_PUPDR_2_1 _pupdr_bit1(ENC1_PIN2)
#define ENC2_PUPDR_1_1 _pupdr_bit1(ENC2_PIN1)
#define ENC2_PUPDR_2_1 _pupdr_bit1(ENC2_PIN2)

void EncoderInit(void){
    PORT->MODER &= ~(ENC1_MODER_1 | ENC1_MODER_2);
    PORT->PUPDR |= (ENC1_PUPDR_1_0 | ENC1_PUPDR_2_0);
    PORT->PUPDR &= ~(ENC1_PUPDR_1_1 | ENC1_PUPDR_2_1);

    PORT->MODER &= ~(ENC2_MODER_1 | ENC2_MODER_2);
    PORT->PUPDR |= (ENC2_PUPDR_1_0 | ENC2_PUPDR_2_0);
    PORT->PUPDR &= ~(ENC2_PUPDR_1_1 | ENC2_PUPDR_2_1);
    /*
    GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7); //Input
    GPIOC->PUPDR |= (GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0);// Pull up
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6_1 | GPIO_PUPDR_PUPDR7_1);

    GPIOC->MODER &= ~(GPIO_MODER_MODER10 | GPIO_MODER_MODER11); //Input
    GPIOC->PUPDR |= (GPIO_PUPDR_PUPDR10_0 | GPIO_PUPDR_PUPDR11_0);// Pull up
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR10_1 | GPIO_PUPDR_PUPDR11_1);
    */
}
void EncoderScan1(uint16_t *counter, uint16_t step, uint16_t _min, uint16_t _max)
{
    uint16_t val;
    static uint8_t old_val=3;
    enum{
        Stable = 0,
        Left,
        Right
    };
    static uint8_t direction = Stable;

    //val = ((GPIOC->IDR & (GPIO_IDR_6 | GPIO_IDR_7))>>6);	// Берем текущее значение
    val = ((PORT->INP_REG & (ENC1_CHECK_1 | ENC1_CHECK_2))>>ENC1_PIN1);

    switch(val)
    {
        case 2:
            if(old_val == 0) direction = Right;
            if(old_val == 3) old_val = 2;
            break;

        case 0:
            old_val = 0;
            break;

        case 1:
            if(old_val == 0) direction = Left;
            if(old_val == 3) old_val = 1;
            break;

        case 3:
            if(direction == Left){
                if(*counter >= step + _min)*counter -= step;
            }
            if(direction == Right){
                if(*counter <= _max - step)*counter += step;
            }
            direction = Stable;
            old_val = 3;
            break;
    }
}

void EncoderScan2(uint16_t *counter, uint16_t step, uint16_t _min, uint16_t _max)
{
    uint16_t val;
    static uint8_t old_val=3;
    enum{
        Stable = 0,
        Left,
        Right
    };
    static uint8_t direction = Stable;

    //val = ((GPIOC->IDR & (GPIO_IDR_6 | GPIO_IDR_7))>>6);	// Берем текущее значение
    val = ((PORT->INP_REG & (ENC2_CHECK_1 | ENC2_CHECK_2))>>ENC2_PIN1);

      switch(val)
    {
        case 2:
            if(old_val == 0) direction = Right;
            if(old_val == 3) old_val = 2;
            break;

        case 0:
            old_val = 0;
            break;

        case 1:
            if(old_val == 0) direction = Left;
            if(old_val == 3) old_val = 1;
            break;

        case 3:
            if(direction == Left){
                if(*counter >= step + _min)*counter -= step;
            }
            if(direction == Right){
                if(*counter <= _max - step)*counter += step;
            }
            direction = Stable;
            old_val = 3;
            break;
    }
}

