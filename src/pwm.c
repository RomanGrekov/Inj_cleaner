#include "pwm.h"
#include "stm32f0xx_tim.h"
#include "../globs.h"


TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

void PWM_init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
     /* GPIOA Clocks enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    /* GPIOA Configuration: Channel 1, 2, 3 and 4 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);

    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /* Channel 1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

        /* TIM1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);

}

void PWM_start(void){
    /* TIM1 clock enable */
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);

    /* TIM1 counter enable */
    TIM_Cmd(TIM1, ENABLE);

    /* TIM1 Main Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void PWM_stop(void){
    /* TIM1 clock enable */
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , DISABLE);

    /* TIM1 counter enable */
    TIM_Cmd(TIM1, DISABLE);

    /* TIM1 Main Output Enable */
    TIM_CtrlPWMOutputs(TIM1, DISABLE);
}

void PWM_update(uint16_t pulse, uint16_t pause, uint16_t presc){

    uint16_t period, _presc, _pulse, _pause;

    uint8_t my_char[8];

    if(presc > 0)_presc = presc - 1;
    else _presc = presc;

    if(pulse > 0){
        _pulse = pulse - 1;
        _pause = pause;
    }
    else{
        _pulse = pulse;
        if(pause > 0)_pause = pause - 1;
        else _pause = pause;
    }

    period = (_pulse + _pause);

    TIM_TimeBaseStructure.TIM_Prescaler = _presc;
    TIM_TimeBaseStructure.TIM_Period = period;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    TIM_OCInitStructure.TIM_Pulse = _pulse;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

}

uint32_t get_tact(uint32_t us , uint32_t my_f_cpu){
    uint32_t k=1;
    if(us >= 1000000) k=1000;
    if(us >= 100000) k=100;
    if(us >= 10000) k=10;
    if(my_f_cpu >= 1000000) return (((us/k) * (my_f_cpu/10000))/100)*k;
    if(my_f_cpu >= 100000) return (((us/k) * (my_f_cpu/1000))/1000)*k;
    if(my_f_cpu >= 10000) return (((us/k) * (my_f_cpu/100))/10000)*k;
    if(my_f_cpu >= 1000) return (((us/k) * (my_f_cpu/10))/100000)*k;
    return (us * (my_f_cpu/10))/100000;

}

void PWM_set(uint32_t pulse, uint32_t pause){
    uint32_t _pulse, _pause, _period;
    uint16_t clear_presc, presc=0;
    uint32_t my_f_cpu;
    uint16_t ost;

    _pulse = get_tact(pulse, F_CPU); //In tacts
    _pause = get_tact(pause, F_CPU); //In tacts
    _period = _pulse + _pause; //In tacts

    if(_period >= 65535){
        ost = _period % 65535;
        clear_presc = _period / 65535;
        if(ost > 0) presc = clear_presc+1;
        else presc = clear_presc;

        my_f_cpu = F_CPU/presc;
        _pulse = get_tact(pulse, my_f_cpu);
        _pause = get_tact(pause, my_f_cpu);
    }

    PWM_update((uint16_t)_pulse, (uint16_t)_pause, presc); //Prescaler = 0
}

