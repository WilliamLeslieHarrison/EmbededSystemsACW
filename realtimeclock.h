/* 
 * File:   realtimeclock.h
 * Author: William Harrison
 *
 * Created on 15 November 2018, 16:04
 */

#ifndef REALTIMECLOCK_H
#define	REALTIMECLOCK_H

#include <xc.h>

#define sclk    RB0
#define i_o     RB4
#define rst     RB5

unsigned char RealTimeClock_buff[7];

void RealTimeClock_write_byte(unsigned char time_tx);
void RealTimeClock_set_time(unsigned char* time);
unsigned char RealTimeClock_read_byte(void);
unsigned char* RealTimeClock_get_time(void);
void RealTimeClock_init(void);

#endif	/* REALTIMECLOCK_H */

