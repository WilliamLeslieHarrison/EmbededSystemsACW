/* 
 * File:   KeyMatrix.h
 * Author: reece
 *
 * Created on 12 November 2018, 16:34
 */

#ifndef KEYMATRIX_H
#define	KEYMATRIX_H

void key_delay(int k);

void InitKeypad();

char keypad_scan();

char switch_scan();

#endif	/* KEYMATRIX_H */

