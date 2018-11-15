#include <pic.h>
#include "KeyMatrix.h"

#define clmn4  RC0
#define clmn3  RC1
#define clmn2 RC2
#define clmn1 RC3
#define row1 RC4
#define row2 RC5
#define row3  RC6
#define row4  RC7

void key_delay(int k){
    unsigned int i,j;
    for (i= 0; i < k; i++)
        for(j = 0; j < 100; j++);
}

void InitKeypad()
{
    PORTC = 0x00;
    TRISC = 0xF;
}
char keypad_scan()
{
    row1 = 1; row2 = 0; row3 = 0; row4 = 0;
    if(clmn1 == 1)
    {
        key_delay(50);
        while(clmn1 == 1)
            return '1';
    }
    if(clmn2 == 1)
    {
       key_delay(50);
       while(clmn2 == 1)
            return '2';
    }
    if(clmn3 == 1)
    {
        key_delay(50);
        while(clmn3 == 1)
            return '3';
    }
    if(clmn4 == 1)
    {
        key_delay(50);
        while(clmn4 == 1)
            return 'A';
    }
    row1 = 0; row2 = 1; row3 = 0; row4 = 0;
    if(clmn1 == 1)
    {
        key_delay(50);
        while(clmn1 == 1)
            return '4';
    }
    if(clmn2 == 1)
    {
        key_delay(50);
        while(clmn2 == 1)
            return '5';
    }
    if(clmn3 == 1)
    {
        key_delay(50);
        while(clmn3 == 1)
            return '6';
    }
    if(clmn4 == 1)
    {
        key_delay(50);
        while(clmn4 == 1)
            return 'B';
    }
    row1 = 0; row2 = 0; row3 = 1; row4 = 0;
    if(clmn1 == 1)
    {
        key_delay(50);
        while(clmn1 == 1)
            return '7';
    }
    if(clmn2 == 1)
    {
        key_delay(50);
        while(clmn2 == 1)
            return '8';
    }
    if(clmn3 == 1)
    {
        key_delay(50);
        while(clmn3 == 1)
            return '9';
    }
    if(clmn4 == 1)
    {
        key_delay(50);
        while(clmn4 == 1)
            return 'C';
    }
    row1 = 0; row2 = 0; row3 = 0; row4 = 1;
    if(clmn1 == 1)
    {
        key_delay(50);
        while(clmn1 == 1)
            return '*';
    }
    if(clmn2 == 1)
    {
        key_delay(50);
        while(clmn2 == 1)
            return '0';
    }
    if(clmn3 == 1)
    {
        key_delay(50);
        while(clmn3 == 1)
            return '#';
    }
    if(clmn4 == 1)
    {
        key_delay(50);
        while(clmn4 == 1)
            return 'D';
    }
    
    return 'n';
}

char switch_scan()
{
    char key = 'n';
    while(key == 'n')
        key = keypad_scan();
    return key;
}
