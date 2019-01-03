/*
 * File:   MainLCD.c
 * Author: reece
 
 * Created on 29 October 2018, 15:41
 */

#include <xc.h>
#include <pic.h>
#include "LCD.h"
#include "KeyMatrix.h"
#include "Thermometer.h"
#include "realtimeclock.h"
#include "Buzzer.h"

//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF /* Low-Voltage (Single-Supply) In-Circuit Serial
Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for
programming)*/
//end config


void get_date_time(int* date_time) {
    unsigned char time[7];
    RealTimeClock_get_time(time);
    date_time[0] = ((int)((time[0] & 0b01110000) >> 4) * 10) + (int)(time[0] & 0b00001111);
    date_time[1] = ((int)((time[1] & 0b01110000) >> 4) * 10) + (int)(time[1] & 0b00001111);    
}

void main() {      
    Init_Buzzer(&PORTB);
    TRISB = 0x00;
    LCD_Init();
    Init_Keypad();
    RealTimeClock_init();
    int test[2];
    char Key = 'n';
    char* Date = "Date: ";
    LCD_Command(0xc);
    LCD_Command(0x01);
    LCD_Command(0x03);   
    LCD_Command(0x38);
    while(1)
    {
    //Key = 'n';
    LCD_Command(0x03); 
    LCD_SendString(Date);  
    LCD_Command(0x14);
    get_date_time(test);
    char* secs;
    char* mins;
    secs = test[0];
    mins = test[1];
    LCD_SendString(secs);
    LCD_SendString(mins);
    LCD_SecondLine();
    Get_Temp();
    Sound(0b00000001);       
    }
}           
