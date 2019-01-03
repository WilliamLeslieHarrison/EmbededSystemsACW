/* 
 * File:   Thermometer.h
 * Author: reece
 *
 * Created on 01 December 2018, 18:53
 */

#ifndef THERMOMETER_H
#define	THERMOMETER_H

#define  uch unsigned char                   //                                          
# define DQ RA0                              //define 18B20 data PORT                    
# define DQ_DIR TRISA0                       //define 18B20 D PORT direct register       
# define DQ_HIGH() DQ_DIR =1                 //set data PORT INPUT                       
# define DQ_LOW() DQ = 0; DQ_DIR = 0         //set data PORT OUTPUT                

#endif

void Delay(char x, char y);

void Display(void);

void Thermometer_Init(void);

void Reset(void);

void Write_Byte(uch val);

uch Read_Byte(void);

void Get_Temp(void);