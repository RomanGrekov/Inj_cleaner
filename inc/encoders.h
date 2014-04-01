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


void EncoderScan1(uint16_t *counter, uint16_t step, uint16_t _min, uint16_t _max);
void EncoderScan1(uint16_t *counter, uint16_t step, uint16_t _min, uint16_t _max);

void EncoderInit(void);

#endif /* ENCODERS_H_INCLUDED */
