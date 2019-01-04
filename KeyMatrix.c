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
int Keypad_Scan(void)
{
    row1 = 1; row2 = 0; row3 = 0; row4 = 0;
    if(clmn1 == 1)
    {
        Key_Delay(50);
        while(clmn1 == 1)
            return 1;
    }
    if(clmn2 == 1)
    {
       Key_Delay(50);
       while(clmn2 == 1)
            return 2;
    }
    if(clmn3 == 1)
    {
        Key_Delay(50);
        while(clmn3 == 1)
            return 3;
    }
    if(clmn4 == 1)
    {
        Key_Delay(50);
        while(clmn4 == 1)
            return 4;
    }
    row1 = 0; row2 = 1; row3 = 0; row4 = 0;
    if(clmn1 == 1)
    {
        Key_Delay(50);
        while(clmn1 == 1)
            return 5;
    }
    if(clmn2 == 1)
    {
        Key_Delay(50);
        while(clmn2 == 1)
            return 6;
    }
    if(clmn3 == 1)
    {
        Key_Delay(50);
        while(clmn3 == 1)
            return 7;
    }
    if(clmn4 == 1)
    {
        Key_Delay(50);
        while(clmn4 == 1)
            return 8;
    }
    row1 = 0; row2 = 0; row3 = 1; row4 = 0;
    if(clmn1 == 1)
    {
        Key_Delay(50);
        while(clmn1 == 1)
            return 9;
    }
    if(clmn2 == 1)
    {
        Key_Delay(50);
        while(clmn2 == 1)
            return 10;
    }
    if(clmn3 == 1)
    {
        Key_Delay(50);
        while(clmn3 == 1)
            return 11;
    }
    if(clmn4 == 1)
    {
        Key_Delay(50);
        while(clmn4 == 1)
            return 12;
    }
    row1 = 0; row2 = 0; row3 = 0; row4 = 1;
    if(clmn1 == 1)
    {
        Key_Delay(50);
        while(clmn1 == 1)
            return 13;
    }
    if(clmn2 == 1)
    {
        Key_Delay(50);
        while(clmn2 == 1)
            return 14;
    }
    if(clmn3 == 1)
    {
        Key_Delay(50);
        while(clmn3 == 1)
            return 15;
    }
    if(clmn4 == 1)
    {
        Key_Delay(50);
        while(clmn4 == 1)
            return 16;
    }
   
        return 0;
}
