#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "power.h"

void power_init(void){
    PORTD &= ~(1<<PORTD7);
    power_release();
}

void power_release(void){
    DDRD &= ~(1<<PORTD7);
}

void power_press(void){
    DDRD |= (1<<PORTD7);
}

void power_press_release(void){
    power_press();
    _delay_ms(1500);
    power_release();
}
