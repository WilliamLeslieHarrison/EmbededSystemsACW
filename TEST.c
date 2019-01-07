#include <xc.h>
#include "TEST.h"
#include "LCD.h"
#include "Thermometer.h"
#include "KeyMatrix.h"

int sendDataCallCount = 0;
int commandCallCount = 0;
int busyCallCount = 0;
int keypadScanCheck = 17;

#define OK_TEST 1
#define ERROR_TEST 0

int TEST_GETTING_TEMP(void)
{
    
}

int TEST_KEYPAD_SCAN(void)
{
    int test = Keypad_Scan();
    
    
}

int TEST_LCD_SENDSTRING(void)
{
    LCD_SendString("TEST");

    if(sendDataCallCount != 4)
    {
        return ERROR_TEST;        
    }
    
    return OK_TEST;
}

int TEST_LCD_BUSY_WITHIN_SENDSTRING(void)
{
    LCD_SendString("BUSY");
    
    if(busyCallCount != 4)
    {
        return ERROR_TEST;
    }
    
    return OK_TEST;
}

void MOCK_COMMANDS(char c)
{
    if(c == 0x14)    LCD_Command(c); 
}

int TEST_COMMANDS(char test)
{
    MOCK_COMMANDS(test);
            
    if(commandCallCount != 1)
    {
        return ERROR_TEST;
    }
    
    return OK_TEST;
}

void TEST_STRING_SCREEN()
{
    int TESTSTRING = TEST_LCD_SENDSTRING();     
    //Sets the returned integer value to a character to be displayed on the LCD
    char stringResult = TESTSTRING + 48;    
    //This is testing to see if the TEST_LCD_SENDSTRING successfully calls 
    //the LCD_SendData function 4 times/ 1 = pass, 0 = fail
    while(1)
    {
    LCD_Command(0x01);
    LCD_Command(0x03);
    LCD_SendData(stringResult);
    }
}

void TEST_COMMAND_SCREEN()
{
    int TESTCOMMANDS = TEST_COMMANDS(0x14);
    char cResult = TESTCOMMANDS + 48;    
    TEST_COMMANDS(0x01);
    TEST_COMMANDS(0x03);
    LCD_SendData(cResult);
}

void TEST_BUSY_SCREEN()
{
        int TESTBUSY = TEST_LCD_BUSY_WITHIN_SENDSTRING();
        LCD_Command(0x01);
        char bResult = TESTBUSY + 48;
        while(1)
        {
        LCD_Command(0x03);
        LCD_SendData(bResult);
        }
}




