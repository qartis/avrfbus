#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "led.h"
#include "nmea.h"
#include "timer.h"
#include "fbus.h"
#include "uart.h"
#include "ts555.h"
#include "multiplex.h"
#include "power.h"

#define GPS_BAUD 9600
#define PHONE_BAUD 115200

uint8_t streq(const char *a, const char *b){
    return strcmp(a,b) == 0;
}

inline uint8_t strstart(const char *buf1, const char *buf2){
    return strncmp(buf1,buf2,strlen(buf2)) == 0;
}

/*
char* capture_and_send(void){
    static char buf[128];
    uint8_t i;
    for (i=0; i<10; i++){
        if (getline_timeout(buf, sizeof(buf), 2) == 0){
            continue;
        }
        buf[strlen(buf)-1] = '\0';
        if (strstart(buf,"$GPRMC") && checksum(buf)){
            return buf;
        }
    }
    return NULL;
}
*/

volatile uint8_t should_ping;

EMPTY_INTERRUPT(PCINT2_vect);

ISR(PCINT0_vect){
    if (ts555_active){
        should_ping = 1;
    }
}

int main(void) {
    wdt_disable();
    led_init();
	uart_init(BAUD(PHONE_BAUD));
    multiplex_init();
    ts555_init();
    power_init();
    sei();
    delay_ms(10);

    printf("tart\n");

    PCMSK2 = 1<<PCINT16;
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    for(;;){
        should_ping = 0;
        ts555_trigger();
        PCICR |=  (1<<PCIE0);
        PCICR |=  (1<<PCIE2);
        led_off();
        sleep_mode();
        //it sleeps several times?
        led_on();
        PCICR &= ~(1<<PCIE2);
        PCICR &= ~(1<<PCIE0);
        ts555_active = 0;

        enum frametype type;
        do {
            timer_start();
            type = fbus_readframe(2);
            timer_disable();
            switch (type){
            case FRAME_READ_TIMEOUT:
                break;
            case FRAME_SMS_RECV:
                printf("from '%s'\nmsg: '%s'\n", unbcd_buf, unpack7_buf);
                if (streq(unpack7_buf, "ping")){
                    uint8_t rc = fbus_sendsms(unbcd_buf, "pong");
                    printf("\nsms: %u\n", rc);
                }
                break;
            case FRAME_NET_STATUS:
                break;
            default:
                printf("strange type %u\n", type);
            }
        } while (type != FRAME_READ_TIMEOUT);
        if (should_ping){
            uint8_t rc = fbus_heartbeat();
            printf("heartbeat: %d\n", rc);
            if (rc == 0){
                rc = fbus_heartbeat();
                if (rc == 0){
                    printf("heartbeat failed! trying to turn phone on\n");
                    power_press_release();
                    _delay_ms(10000);
                    printf("done\n");
                }
            }
        }
    }
}
