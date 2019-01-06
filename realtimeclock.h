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

int __RealTimeClock_buff[7];


int __hex_to_int(unsigned char hex);
unsigned char __int_to_hex(int integer);


void RealTimeClock_write_byte(unsigned char time_tx);
void RealTimeClock_set_burst_time(int* time);
unsigned char RealTimeClock_read_byte(void);
int* RealTimeClock_get_burst_time(void);

void RealTimeClock_init(void);

int RealTimeClock_get_seconds(void);
void RealTimeClock_set_seconds(int seconds);

int RealTimeClock_get_minutes(void);
int RealTimeClock_set_minutes(int minutes);

int RealTimeClock_get_hours(void);
void RealTimeClock_set_hours(int hours);

int RealTimeClock_get_day_of_month(void);
void RealTimeClock_set_day_of_month(int day);

int RealTimeClock_get_month(void);
void RealTimeClock_set_month(int month);

int RealTimeClock_get_day_of_week(void);
void RealTimeClock_set_day_of_week(int day_of_week);

int RealTimeClock_get_year(void);
void RealTimeClock_set_year(int year);




#endif	/* REALTIMECLOCK_H */
