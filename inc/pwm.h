#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDED
#include "stm32f0xx.h"

void PWM_init(void);
void PWM_update(uint16_t pulse, uint16_t pause, uint16_t presc);
uint32_t get_tact(uint32_t us , uint32_t my_f_cpu);
void PWM_set(uint32_t pulse, uint32_t pause);
void PWM_start(void);
void PWM_stop(void);

#endif /* PWM_H_INCLUDED */
