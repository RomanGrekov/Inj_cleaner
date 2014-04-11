#include "test1.h"
#include "../slow_timer/slow_timer.h"
#include "../flash/flash.h"

//=======================Sys variables===============================
void UpdateLcd(void);
void EncoderScan_1(void);
void EncoderScan_2(void);
void LED8(uint8_t state);
void handle_buttons(void);
void get_r(void);
void get_f(void);
void Stop(void);
void var_init(void);
uint8_t get_time_symb(uint32_t value);

uint16_t duration=0;
uint16_t dur_values[8]={0,5,10,30,60,120,300,600};
uint8_t chars[10];

Encoder enc1_struct;
Encoder enc2_struct;

struct SavedDomain SysConf;
uint8_t test_n;

uint8_t button, sm_changed=1, exit=0, t_1=0, t_2=0, t_3=0, t_4=0;
uint8_t pwm_state=0;
uint8_t freq_rev=0;

//====================================================================================

void test(void){
    duration=0;

    button=0;
    sm_changed=1;
    exit=0;
    t_1=0; t_2=0; t_3=0; t_4=0;
    pwm_state=0;
    freq_rev=0;

    lcd_clrscr();

    PWM_init();

    EncoderInit();

    //clean_btn();

    t_1=Slow_Timer_Add(tm_Repeat, 100, UpdateLcd);
    t_2=Slow_Timer_Add(tm_Repeat, 1, EncoderScan_1);
    t_3=Slow_Timer_Add(tm_Repeat, 1, EncoderScan_2);

    while(exit == 0)
    {


    }
    Slow_Timer_Delete(t_1);
    Slow_Timer_Delete(t_2);
    Slow_Timer_Delete(t_3);

}

void UpdateLcd(void){
    char my_char[8]={0};

    handle_buttons();

    if(sm_changed){
        sm_changed=0;
        lcd_clrscr();

        itoa_fix(dur_values[duration], 10, 3, chars);
        lcd_prints("D:");
        lcd_prints(chars);

        if(freq_rev == 0){
            get_f();
            lcd_prints(" F:");
            lcd_prints(chars);
        }
        else{
            get_r();
            lcd_prints(" R:");
            lcd_prints(chars);

        }

        lcd_goto(2,0);
        lcd_prints("H:");
        itoa_fix(enc1_struct.value, 10, 4, my_char);
        lcd_prints(my_char);
        lcd_putc(get_time_symb(enc1_struct.coef));

        lcd_prints("  ");
        lcd_prints("L:");
        itoa_fix(enc2_struct.value, 10, 4, my_char);
        lcd_prints(my_char);
        lcd_putc(get_time_symb(enc2_struct.coef));
    }
}

void EncoderScan_1(void){
    EncoderScan(&enc1_struct);
    if(enc1_struct.value != enc1_struct.old_value){
        sm_changed=1;
        PWM_set(enc1_struct.value*enc1_struct.coef, enc2_struct.value*enc2_struct.coef);
        *(SysConf.enc+test_n*8)=enc1_struct.value;
    }
    enc1_struct.old_value = enc1_struct.value;
}

void EncoderScan_2(void){
    EncoderScan(&enc2_struct);
    if(enc2_struct.value != enc2_struct.old_value){
        sm_changed=1;
        PWM_set(enc1_struct.value*enc1_struct.coef, enc2_struct.value*enc2_struct.coef);
        *(SysConf.enc+test_n*8+4)=enc2_struct.value;
    }
    enc2_struct.old_value = enc2_struct.value;
}

void LED9(uint8_t state){
    if(state) GPIOC->ODR |= GPIO_Pin_9 ;
    if(!state) GPIOC->ODR &= ~GPIO_Pin_9;
}

void handle_buttons(void){
    button = read_btn();

    if(button){
        sm_changed=1;
        if(button == BTN4){
            pwm_state = ~pwm_state;
            if(pwm_state){
                PWM_start();
                if(duration != 0)t_4 = Slow_Timer_Add(tm_Once, dur_values[duration]*1000, Stop);
            }
            else{
                Stop();
                if(t_4 != 0){
                    Slow_Timer_Delete(t_4);
                    t_4=0;
                }
            }
            LED9(pwm_state);
        }
        if(button == BTN1){
            if(duration > 0)duration--;
            else duration = ((sizeof(dur_values)/2)-1);
        }
        if(button == BTN3){
            if(duration < ((sizeof(dur_values)/2)-1))duration++;
            else duration = 0;
        }
        if(button == BTN2){
            if(!pwm_state)exit=1;
        }
        if(button == ENC2_BTN)freq_rev = ~freq_rev;

        if(button == ENC1_BTN){
            flash_write_struct(&SysConf, sizeof(SysConf));
        }
        //clean_btn();
    }
}

void get_f(void){
    uint32_t f=0, ost=0;
    if((enc1_struct.value*enc1_struct.coef + enc2_struct.value*enc2_struct.coef) > 0){
        f = 1000000/(enc1_struct.value*enc1_struct.coef+enc2_struct.value*enc2_struct.coef);
    }
    else f=0;

    if(f <= 1000){
        itoa_fix(f, 10, 3, chars);
        chars[3]='H';
        chars[4]='z';
        chars[5]=0;
        return;
    }
      if(1000 <= f <= 1000000){
        ost = f%1000;
        f = f/1000;
        itoa_fix(f, 10, 3, chars);
        chars[3]=',';
        ost = ost/100;
        itoa_fix(ost, 10, 1, chars+4);
        chars[5]='K';
        chars[6]='H';
        chars[7]='z';
        chars[8]=0;
        return;
    }
        if(1000000 <= f <= 100000000){
        ost = f%1000000;
        f = f/1000000;
        itoa_fix(f, 10, 3, chars);
        chars[3]=',';
        ost = ost/10000000;
        itoa_fix(ost, 10, 1, chars+4);
        chars[5]='M';
        chars[6]='H';
        chars[7]='z';
        chars[8]=0;
        return;
    }
}

void get_r(void){
    uint32_t r=0;
    if((enc1_struct.value*enc1_struct.coef + enc2_struct.value*enc2_struct.coef) > 0){
        r = 1000000/(enc1_struct.value*enc1_struct.coef+enc2_struct.value*enc2_struct.coef)*30;
    }
    else r=0;

    itoa_fix(r, 10, 8, chars);
    return;
}

void Stop(void){
    PWM_stop();
    pwm_state=0;
    LED9(pwm_state);
}

uint8_t get_time_symb(uint32_t value){
        if(value < 1000)return 'u';
        if(1000 <= value && value < 1000000)return  'm';
        if(value >= 1000000)return 'S';
}


///=======Tests===========================================================
void drain_test1(void){
test_n=0;
flash_read_struct(&SysConf, sizeof(SysConf));

enc1_struct.value=*(SysConf.enc+test_n*8);
enc1_struct.old_value=0;
enc1_struct.step=10;
enc1_struct.min=1;
enc1_struct.max=2000;
enc1_struct.coef=1;
enc1_struct.pin1=6;
enc1_struct.pin2=7;
enc1_struct.old_state=3;
enc1_struct.direction=0;

enc2_struct.value=*(SysConf.enc+test_n*8+4);
enc2_struct.old_value=0;
enc2_struct.step=1;
enc2_struct.min=1;
enc2_struct.max=1000;
enc2_struct.coef=1000;
enc2_struct.pin1=10;
enc2_struct.pin2=11;
enc2_struct.old_state=3;
enc2_struct.direction=0;

test();
}

void drain_test2(void){
test_n=1;
flash_read_struct(&SysConf, sizeof(SysConf));

enc1_struct.value=*(SysConf.enc+test_n*8);
enc1_struct.old_value=0;
enc1_struct.step=10;
enc1_struct.min=1;
enc1_struct.max=2000;
enc1_struct.coef=1;
enc1_struct.pin1=6;
enc1_struct.pin2=7;
enc1_struct.old_state=3;
enc1_struct.direction=0;

enc2_struct.value=*(SysConf.enc+test_n*8+4);
enc2_struct.old_value=0;
enc2_struct.step=10;
enc2_struct.min=1;
enc2_struct.max=1000;
enc2_struct.coef=1000;
enc2_struct.pin1=10;
enc2_struct.pin2=11;
enc2_struct.old_state=3;
enc2_struct.direction=0;

test();
}

void cavitation_test1(void){
test_n=2;
flash_read_struct(&SysConf, sizeof(SysConf));

enc1_struct.value=*(SysConf.enc+test_n*8);
enc1_struct.old_value=0;
enc1_struct.step=1;
enc1_struct.min=1;
enc1_struct.max=5000;
enc1_struct.coef=1;
enc1_struct.pin1=6;
enc1_struct.pin2=7;
enc1_struct.old_state=3;
enc1_struct.direction=0;

enc2_struct.value=*(SysConf.enc+test_n*8+4);
enc2_struct.old_value=0;
enc2_struct.step=1;
enc2_struct.min=1;
enc2_struct.max=5000;
enc2_struct.coef=1;
enc2_struct.pin1=10;
enc2_struct.pin2=11;
enc2_struct.old_state=3;
enc2_struct.direction=0;

test();
}

void cavitation_test2(void){
test_n=3;
flash_read_struct(&SysConf, sizeof(SysConf));

enc1_struct.value=*(SysConf.enc+test_n*8);
enc1_struct.old_value=0;
enc1_struct.step=10;
enc1_struct.min=1;
enc1_struct.max=5000;
enc1_struct.coef=1;
enc1_struct.pin1=6;
enc1_struct.pin2=7;
enc1_struct.old_state=3;
enc1_struct.direction=0;

enc2_struct.value=*(SysConf.enc+test_n*8+4);
enc2_struct.old_value=0;
enc2_struct.step=10;
enc2_struct.min=1;
enc2_struct.max=5000;
enc2_struct.coef=1;
enc2_struct.pin1=10;
enc2_struct.pin2=11;
enc2_struct.old_state=3;
enc2_struct.direction=0;

test();
}

void balance_test1(void){
test_n=4;
flash_read_struct(&SysConf, sizeof(SysConf));

enc1_struct.value=*(SysConf.enc+test_n*8);
enc1_struct.old_value=0;
enc1_struct.step=1;
enc1_struct.min=1;
enc1_struct.max=50;
enc1_struct.coef=1000;
enc1_struct.pin1=6;
enc1_struct.pin2=7;
enc1_struct.old_state=3;
enc1_struct.direction=0;

enc2_struct.value=*(SysConf.enc+test_n*8+4);
enc2_struct.old_value=0;
enc2_struct.step=1;
enc2_struct.min=1;
enc2_struct.max=50;
enc2_struct.coef=1000;
enc2_struct.pin1=10;
enc2_struct.pin2=11;
enc2_struct.old_state=3;
enc2_struct.direction=0;

test();
}

void balance_test2(void){
test_n=5;
flash_read_struct(&SysConf, sizeof(SysConf));

enc1_struct.value=*(SysConf.enc+test_n*8);
enc1_struct.old_value=0;
enc1_struct.step=1;
enc1_struct.min=1;
enc1_struct.max=50;
enc1_struct.coef=1000;
enc1_struct.pin1=6;
enc1_struct.pin2=7;
enc1_struct.old_state=3;
enc1_struct.direction=0;

enc2_struct.value=*(SysConf.enc+test_n*8+4);
enc2_struct.old_value=0;
enc2_struct.step=1;
enc2_struct.min=1;
enc2_struct.max=50;
enc2_struct.coef=1000;
enc2_struct.pin1=10;
enc2_struct.pin2=11;
enc2_struct.old_state=3;
enc2_struct.direction=0;

test();
}

void balance_test3(void){
test_n=6;
flash_read_struct(&SysConf, sizeof(SysConf));

enc1_struct.value=*(SysConf.enc+test_n*8);
enc1_struct.old_value=0;
enc1_struct.step=1;
enc1_struct.min=1;
enc1_struct.max=50;
enc1_struct.coef=1000;
enc1_struct.pin1=6;
enc1_struct.pin2=7;
enc1_struct.old_state=3;
enc1_struct.direction=0;

enc2_struct.value=*(SysConf.enc+test_n*8+4);
enc2_struct.old_value=0;
enc2_struct.step=10;
enc2_struct.min=10;
enc2_struct.max=1000;
enc2_struct.coef=1000;
enc2_struct.pin1=10;
enc2_struct.pin2=11;
enc2_struct.old_state=3;
enc2_struct.direction=0;

test();
}

void balance_test4(void){
test_n=7;
flash_read_struct(&SysConf, sizeof(SysConf));

enc1_struct.value=*(SysConf.enc+test_n*8);
enc1_struct.old_value=0;
enc1_struct.step=1;
enc1_struct.min=1;
enc1_struct.max=50;
enc1_struct.coef=1000;
enc1_struct.pin1=6;
enc1_struct.pin2=7;
enc1_struct.old_state=3;
enc1_struct.direction=0;

enc2_struct.value=*(SysConf.enc+test_n*8+4);
enc2_struct.old_value=0;
enc2_struct.step=10;
enc2_struct.min=10;
enc2_struct.max=1000;
enc2_struct.coef=1000;
enc2_struct.pin1=10;
enc2_struct.pin2=11;
enc2_struct.old_state=3;
enc2_struct.direction=0;

test();
}

