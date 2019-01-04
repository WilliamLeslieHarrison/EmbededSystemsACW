/* 
 * File:   KeyMatrix.h
 * Author: reece
 *
 * Created on 12 November 2018, 16:34
 */

#ifndef KEYMATRIX_H
#define	KEYMATRIX_H

#define clmn4  RC0
#define clmn3  RC1
#define clmn2 RC2
#define clmn1 RC3
#define row1 RC4
#define row2 RC5
#define row3  RC6
#define row4  RC7

#endif	/* KEYMATRIX_H */
void Key_Delay(int k);

void Init_Keypad(void);

int Keypad_Scan(void);




