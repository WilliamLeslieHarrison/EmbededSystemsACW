/* 
 * File:   TEST.h
 * Author: Reece Thompson
 *
 * Created on 07 January 2019, 13:49
 */

#ifndef TEST_H
#define	TEST_H
#define LCD_COMMAND_TEST(X) commandCallCount++
#define LCD_BUSY() busyCallCount++
#define KEYPAD_SCAN() keypadScanCheck
#define GET_TEMP() getTempTest
#define LCD_SEND_DATA(X) sendDataCallCount++
#else
#define LCD_SEND_DATA(X) LCD_SendData(X)
#define LCD_COMMAND_TEST(X) LCD_Command(X)
#define LCD_BUSY() LCD_Busy()
#endif

int TEST_GETTING_TEMP(void);

int TEST_KEYPAD_SCAN(void);

int TEST_LCD_SENDSTRING(void);

int TEST_LCD_BUSY_WITHIN_SENDSTRING(void);

void MOCK_COMMANDS(char c);

int TEST_COMMANDS(char test);

void TEST_STRING_SCREEN(void);

void TEST_COMMAND_SCREEN();

void TEST_BUSY_SCREEN();