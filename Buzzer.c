#include<xc.h>
#include"Buzzer.h"


//Initialise the buzzer by passing through a PORT the user wants to use
void Buzzer_Init()
{
    TRISB1 = 0;
}

//Change the sound to be on
void SoundOn()
{
    buzzer = 1;
}

//Change the sound to be off
void SoundOff()
{
    buzzer = 0;
}
