#include <xc.h>
#include "realtimeclock.h"


int __hex_to_int(unsigned char hex) {
    int t = 0;
    for(int i = 0; i < hex; ++i) {
        ++t;
        if(t && t % 10 == 0)
            i+=6;
    }
    return t;
}

unsigned char __int_to_hex(int integer) {
    int t = 0;
    for(int i = 0; i < integer; ++i) {
        ++t;
        if(i && i % 10 == 0) {
            t+=6;
        }
    }
    return t;
}

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

void RealTimeClock_set_burst_time(int* time) {
    rst = 1;
    RealTimeClock_write_byte(0xbe);
    for(int i = 0; i < 8; ++i) {
        RealTimeClock_write_byte(__int_to_hex(time[i]));
    }
    rst = 0;  
}

int* RealTimeClock_get_burst_time(void) {
    rst = 1;
    RealTimeClock_write_byte(0xbf);
    for(int i = 0; i < 7; ++i) {
        __RealTimeClock_buff[i] = __hex_to_int(RealTimeClock_read_byte());
    }
    rst = 0;
    return __RealTimeClock_buff;
}

void RealTimeClock_init(void) {
    ADCON1 = 0x06;
    TRISB0 = 0;
    TRISB4 = 0;
    TRISB5 = 0;
    sclk = 0;
    rst = 1;
    RealTimeClock_write_byte(0x8e);
    RealTimeClock_write_byte(0x00);
    rst = 0;
}

int RealTimeClock_get_seconds(void) {
    RealTimeClock_write_byte(0x81);
    return __hex_to_int(RealTimeClock_read_byte());
}

int RealTimeClock_get_minutes(void) {
    RealTimeClock_write_byte(0x83);
    return __hex_to_int(RealTimeClock_read_byte());
}

int RealTimeClock_get_hours(void) {
    RealTimeClock_write_byte(0x85);
    return __hex_to_int(RealTimeClock_read_byte());
}

