/*
 * File:   main.c
 * Author: 515817
 *
 * Created on 15 November 2018, 15:24
 */

//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF
//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON
// Power - up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF
// Brown - out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low - Voltage (Single - Supply) In - Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config


#include <xc.h>

void main(void) {
    return;
}
