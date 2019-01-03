#include<pic.h>
#include"Buzzer.h"

void Init_Buzzer()
{
    PORTC = 0x00;
    TRISC = 0x00;
}

void Sound()
{
    if(buzzer == 0)
    {
        RC2 = 0;
        buzzer = 1;
    }
    else if(buzzer == 1)
    {
        RC2 = 1;
        buzzer = 0;
    }
}
