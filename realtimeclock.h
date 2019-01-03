/* 
 * File:   realtimeclock.h
 * Author: William Harrison
 *
 * Created on 15 November 2018, 16:04
 */

#ifndef REALTIMECLOCK_H
#define	REALTIMECLOCK_H

#define i_o   RB4                      //1302I_O           
#define sclk  RB0                      //1302 clock        
#define rst   RB5

#define BURST_MODE_READ 0xbf;

void ReadTimeClock_write_byte(unsigned char time_tx);
void ReadTimeClock_set_time(unsigned char* time);
void ReadTimeClock_read_byte(unsigned char time_rx);
void ReadTimeClock_get_time(unsigned char* time);
void RealTimeClock_init(void);

#endif	/* REALTIMECLOCK_H */

