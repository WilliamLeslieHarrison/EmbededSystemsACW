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

void RealTimeClock_read_byte(unsigned char time_rx) {
    TRISB4 = 1;
    for(int i = 0; i < 8; ++i) {
        sclk = 0;
        time_rx = time_rx  >> 1;
        time_rx = time_rx | (((unsigned char)i_o) << 7);
        sclk = 1;
    }
    TRISB4 = 0;
    sclk = 0;
}

void RealTimeClock_set_time(unsigned char* time) {
    rst = 1;
    RealTimeClock_write_byte(0xbe);
    for(int i = 0; i < 8; ++i) {
        RealTimeClock_write_byte(time[i]);
    }
    rst = 0;  
}

void RealTimeClock_get_time(unsigned char* time) {
    rst = 1;
    RealTimeClock_write_byte(0xbf);
    for(int i = 0; i < 7; ++i) {
        RealTimeClock_read_byte(time[i]);
    }
    rst = 0;
}

void RealTimeClock_init(void) {
    TRISB = 0x00;
    sclk = 0;
    rst = 0;
    RealTimeClock_write_byte(0x8e);   
    RealTimeClock_write_byte(0x00);
    rst = 0;
}