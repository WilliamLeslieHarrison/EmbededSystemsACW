/* 
 * File:   LCD.h
 * Author: reece
 *
 * Created on 07 November 2018, 15:30
 */

#ifndef LCD_H //Include guard
#define	LCD_H

#define LCD_data    PORTD //LCD 8 bit data port
#define LCD_DB7     RD7 //busy flag in
#define LCD_rs      RA1
#define LCD_rw      RA2
#define LCD_en      RA3

#endif

void LCD_busy(); //Declaring the LCD_busy function

void LCD_init(); //Declaring the initialization for the LCD's

void LCD_command(unsigned char c); //Declaring the commands for the LCDs

void LCD_secondline();

void LCD_senddata(unsigned char c); //Declaring the method for sending data

void LCD_sendstring(const char *string); //Declaring the send string function



