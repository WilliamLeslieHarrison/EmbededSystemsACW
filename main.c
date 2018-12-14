/*
 * File:   MainLCD.c
 * Author: reece
 *
 * Created on 29 October 2018, 15:41
 */

#include <xc.h>
#include <pic.h>
#include "LCD.h"//Declaring the header here
#include "KeyMatrix.h"
#include "Thermometer.h"
#include "realtimeclock.h"

//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF /* Low-Voltage (Single-Supply) In-Circuit Serial
Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for
programming)*/
//end config

void main() {               
    LCD_init();
    char* c = "hello";
    LCD_command(0x0f);
    LCD_command(0x01);
    LCD_command(0x03);
    LCD_sendstring(c);
    while(1){};
}                      