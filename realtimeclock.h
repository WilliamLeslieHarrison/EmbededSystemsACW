/* 
 * File:   realtimeclock.h
 * Author: William Harrison
 *
 * Created on 15 November 2018, 16:04
 */

#ifndef REALTIMECLOCK_H
#define	REALTIMECLOCK_H

#include <xc.h>

#define i_o   RB4                      //1302I_O           
#define sclk  RB0                      //1302 clock        
#define rst   RB5

#define BURST_MODE_READ 0xbf;

void RealTimeClock_write_byte(unsigned char time_tx);
void RealTimeClock_set_time(unsigned char* time);
void RealTimeClock_read_byte(unsigned char time_rx);
void RealTimeClock_get_time(unsigned char* time);
void RealTimeClock_init(void);

#endif	/* REALTIMECLOCK_H */

