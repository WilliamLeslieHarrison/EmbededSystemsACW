#include <pic.h>
#include "KeyMatrix.h"

//Simple delay
void Key_Delay(int k){
    unsigned int i,j;
    for (i= 0; i < k; i++)
        for(j = 0; j < 100; j++);
}
//Initialise the Keypad
void Init_Keypad(void)
{
    PORTC = 0x00;
    TRISC = 0xF;
}
//This checks in each row which columns are active and returns a character
char Keypad_Scan(void)
{
    row1 = 1; row2 = 0; row3 = 0; row4 = 0;
    if(clmn1 == 1)
    {
        Key_Delay(50);
        while(clmn1 == 1)
            return '1';
    }
    if(clmn2 == 1)
    {
       Key_Delay(50);
       while(clmn2 == 1)
            return '2';
    }
    if(clmn3 == 1)
    {
        Key_Delay(50);
        while(clmn3 == 1)
            return '3';
    }
    if(clmn4 == 1)
    {
        Key_Delay(50);
        while(clmn4 == 1)
            return '4';
    }
    row1 = 0; row2 = 1; row3 = 0; row4 = 0;
    if(clmn1 == 1)
    {
        Key_Delay(50);
        while(clmn1 == 1)
            return '5';
    }
    if(clmn2 == 1)
    {
        Key_Delay(50);
        while(clmn2 == 1)
            return '6';
    }
    if(clmn3 == 1)
    {
        Key_Delay(50);
        while(clmn3 == 1)
            return '7';
    }
    if(clmn4 == 1)
    {
        Key_Delay(50);
        while(clmn4 == 1)
            return '8';
    }
    row1 = 0; row2 = 0; row3 = 1; row4 = 0;
    if(clmn1 == 1)
    {
        Key_Delay(50);
        while(clmn1 == 1)
            return '9';
    }
    if(clmn2 == 1)
    {
        Key_Delay(50);
        while(clmn2 == 1)
            return '0';
    }
    if(clmn3 == 1)
    {
        Key_Delay(50);
        while(clmn3 == 1)
            return 'a';
    }
    if(clmn4 == 1)
    {
        Key_Delay(50);
        while(clmn4 == 1)
            return 'b';
    }
    row1 = 0; row2 = 0; row3 = 0; row4 = 1;
    if(clmn1 == 1)
    {
        Key_Delay(50);
        while(clmn1 == 1)
            return 'c';
    }
    if(clmn2 == 1)
    {
        Key_Delay(50);
        while(clmn2 == 1)
            return 'd';
    }
    if(clmn3 == 1)
    {
        Key_Delay(50);
        while(clmn3 == 1)
            return 'e';
    }
    if(clmn4 == 1)
    {
        Key_Delay(50);
        while(clmn4 == 1)
            return 'f';
    }
    
        return 'n';
}
//This goes through the scan and returns a char with that dedicated button
char Switch_Scan(void)
{
    char key = 'n';
    while(key == 'n')
        key = Keypad_Scan();
    return key;
}
