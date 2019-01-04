#include<xc.h>
#include"Buzzer.h"

char* PORT;

//Initialise the buzzer by passing through a PORT the user wants to use
void Init_Buzzer(char* Port)
{
    *Port = 0x00;    
    *PORT = *Port;       
}

//Change the sound to be on or off (depending what the previous state was)
void Sound(char Mask)
{
    *PORT = Mask;
    PORT = ~*PORT;
}
