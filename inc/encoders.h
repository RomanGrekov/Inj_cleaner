#ifndef ENCODERS_H_INCLUDED
#define ENCODERS_H_INCLUDED
#include <stdint.h>
#include "stm32f0xx.h"

#define PORT GPIOC
#define INP_REG IDR

#define ENC1_PIN1 6
#define ENC1_PIN2 7

#define ENC2_PIN1 10
#define ENC2_PIN2 11


typedef struct{
    uint32_t value;
    uint32_t old_value;
    uint16_t step;
    uint16_t min;
    uint16_t max;
    uint16_t coef;
    uint32_t pin1;
    uint32_t pin2;
    uint8_t old_state;
    uint8_t direction;
}Encoder;

void EncoderScan(Encoder *enc_struct);

void EncoderInit(void);

#endif /* ENCODERS_H_INCLUDED */
