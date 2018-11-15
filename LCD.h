/* 
 * File:   LCD.h
 * Author: reece
 *
 * Created on 07 November 2018, 15:30
 */

#ifndef LCD_H //Include guard
#define	LCD_H

void LCD_busy(); //Declaring the LCD_busy function

void LCD_init(); //Declaring the initialization for the LCD's

void LCD_command(unsigned char c); //Declaring the commands for the LCDs

void LCD_secondline();

void LCD_senddata(unsigned char c); //Declaring the method for sending data

void LCD_sendstring(const char *string); //Declaring the send string function



#endif



