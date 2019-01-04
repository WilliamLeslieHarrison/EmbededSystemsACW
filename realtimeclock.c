#include <xc.h>
#include "realtimeclock.h"

void RealTimeClock_write_byte(unsigned char time_tx) {
    for(int i = 0; i < 8; ++i) {
        i_o = 0;
        sclk = 0;
        if (time_tx&0x01) {
            i_o = 1;
        }
        time_tx = time_tx >> 1;
        sclk = 1;
    }
    sclk = 0;
}

unsigned char RealTimeClock_read_byte(void) {
    unsigned char time_rx;
    TRISB4 = 1;
    for(int i = 0; i < 8; ++i) {
        sclk = 0;
        //time_rx = time_rx  >> 1;
        time_rx = time_rx | (((unsigned char)i_o) << i);
        sclk = 1;
    }
    sclk = 0;
    TRISB4 = 0;
    return time_rx;
}

void RealTimeClock_set_time(unsigned char* time) {
    rst = 1;
    RealTimeClock_write_byte(0xbe);
    for(int i = 0; i < 8; ++i) {
        RealTimeClock_write_byte(time[i]);
    }
    rst = 0;  
}

unsigned char* RealTimeClock_get_time(void) {
    rst = 1;
    RealTimeClock_write_byte(0xbf);
    for(int i = 0; i < 7; ++i) {
        RealTimeClock_buff[i] = RealTimeClock_read_byte();
    }
    rst = 0;
    return RealTimeClock_buff;
}

void RealTimeClock_init(void) {
    TRISB0 = 0;
    TRISB4 = 0;
    TRISB5 = 0;
    sclk = 0;
    rst = 1;
    RealTimeClock_write_byte(0x8e);
    RealTimeClock_write_byte(0x00);
    rst = 0;
}
