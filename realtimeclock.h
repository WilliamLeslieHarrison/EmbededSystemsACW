/* 
 * File:   realtimeclock.h
 * Author: 515817
 *
 * Created on 15 November 2018, 16:04
 */

#ifndef REALTIMECLOCK_H
#define	REALTIMECLOCK_H

#define i_o     RB4                      //1302I_O           
#define sclk    RB0                      //1302 clock        
#define rst     RB5                      //1302 enable bit   
 

 unsigned char time_rx;

void ds1302_init();                             //DS1302 initilize subroutine.
void set_time(char *table);                                //set time subroutine.
void get_time(char *table);                                //get time subroutine.
void time_write_byte(unsigned char time_tx);    //write one byte subroutine.
unsigned char  time_read_byte();                //read one byte subroutine.


#endif	/* REALTIMECLOCK_H */

